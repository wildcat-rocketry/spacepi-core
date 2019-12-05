#define _DEFAULT_SOURCE
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include <spacepi.h>

#ifndef WORKER_THREAD_TIMEOUT
#define WORKER_THREAD_TIMEOUT 2000
#endif
#ifndef WORKER_THREAD_MAX_QUEUE
#define WORKER_THREAD_MAX_QUEUE 10
#endif
#ifndef THREAD_YEILD_THRESHOLD
#define THREAD_YIELD_THRESHOLD 0.5
#endif
#ifndef THREAD_YIELD_MARGIN
#define THREAD_YIELD_MARGIN 0.1
#endif
#ifndef THREAD_YIELD_ADAPTIVENESS
#define THREAD_YIELD_ADAPTIVENESS 0.01
#endif

//#define WORKER_THREAD_DEBUG

typedef struct _queue_entry queue_entry_t;
struct _queue_entry {
    void (*trampoline)(void *context);
    void *context;
    queue_entry_t *next;
};

static pthread_mutex_t queue_mutex;
static pthread_mutex_t worker_mutex;
static pthread_cond_t worker_cond;
static volatile queue_entry_t *head = NULL;
static volatile queue_entry_t *tail = NULL;
static volatile int queue_count = 0;

static void *worker_thread(void *ctx);

int thread_pool_init(void) {
    static int need_init = 1;
    if (need_init) {
        CHECK_ERROR(pthread_mutex_init, &queue_mutex, NULL);
        CHECK_ERROR_JUMP(free_queue_mutex, pthread_mutex_init, &worker_mutex, NULL);
        CHECK_ERROR_JUMP(free_worker_mutex, pthread_cond_init, &worker_cond, NULL);
        pthread_t thread;
        CHECK_ERROR_JUMP(free_worker_cond, pthread_create, &thread, NULL, worker_thread, (void *) 1);
        need_init = 0;
    }
    return 0;

    free_worker_cond:
    pthread_cond_destroy(&worker_cond);
    free_worker_mutex:
    pthread_mutex_destroy(&worker_mutex);
    free_queue_mutex:
    pthread_mutex_destroy(&queue_mutex);
    RETURN_REPORTED_ERROR();
}

int thread_enqueue(void (*trampoline)(void *context), void *context) {
    CHECK_ALLOC_DEF(node, queue_entry_t);
    node->trampoline = trampoline;
    node->context = context;
    node->next = NULL;
    CHECK_ERROR_JUMP(free_node, pthread_mutex_lock, &queue_mutex);
    if (tail) {
        tail->next = node;
        tail = node;
    } else {
        head = tail = node;
        pthread_cond_signal(&worker_cond);
    }
    int n = ++queue_count;
    pthread_mutex_unlock(&queue_mutex);
    if (n > WORKER_THREAD_MAX_QUEUE) {
        pthread_t thread;
        CHECK_ERROR(pthread_create, &thread, NULL, worker_thread, NULL);
    }
    return 0;
    
    free_node:
    free(node);
    RETURN_REPORTED_ERROR();
}

static void *worker_thread(void *ctx) {
#ifdef WORKER_THREAD_DEBUG
    if (ctx) {
        fputs("Main worker thread spawned.\n", stderr);
    } else {
        fputs("Helper worker thread spawned.\n", stderr);
    }
#endif
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        if (head) {
            queue_entry_t *node = (queue_entry_t *) head;
            if (head == tail) {
                head = tail = NULL;
            } else {
                head = head->next;
            }
            --queue_count;
            pthread_mutex_unlock(&queue_mutex);
            node->trampoline(node->context);
        } else {
            pthread_mutex_unlock(&queue_mutex);
            struct timespec tm = {
                .tv_sec = WORKER_THREAD_TIMEOUT / 1000,
                .tv_nsec = (WORKER_THREAD_TIMEOUT % 1000) * 1000000
            };
            int e = pthread_mutex_timedlock(&worker_mutex, &tm);
            if (e == ETIMEDOUT) {
                break;
            }
            pthread_cond_timedwait(&worker_cond, &worker_mutex, &tm);
            pthread_mutex_unlock(&worker_mutex);
        }
    }
#ifdef WORKER_THREAD_DEBUG
    fputs("Helper worker thread died.\n", stderr);
#endif
}

void thread_block(void) {
    while (1) {
        pause();
    }
}

int thread_yield(int millis) {
    static float fraction = 1;
    static int proc_count = 0;
    if (proc_count <= 0) {
        proc_count = get_nprocs_conf();
        if (proc_count <= 0) {
            spacepi_perror("get_nprocs_conf", __FILE__, __LINE__);
            RETURN_REPORTED_ERROR();
        }
    }
    double loadavg;
    CHECK_ERROR(getloadavg, &loadavg, 1);
    loadavg /= proc_count;
    if (loadavg > THREAD_YIELD_THRESHOLD + THREAD_YIELD_MARGIN) {
        fraction *= 1 + THREAD_YIELD_ADAPTIVENESS;
    } else if (loadavg < THREAD_YIELD_THRESHOLD - THREAD_YIELD_MARGIN) {
        fraction *= 1 - THREAD_YIELD_ADAPTIVENESS;
    }
    if (fraction > 1) {
        fraction = 1;
    }
    millis *= fraction;
    sleep(millis / 1000);
    usleep(1000 * (millis % 1000));
}
