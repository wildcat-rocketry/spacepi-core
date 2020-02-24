#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <spacepi.h>

int timer_init(struct timespec *timer) {
    CHECK_ERROR(clock_gettime, CLOCK_REALTIME, timer);
    return 0;
}

int sleep_until(struct timespec *timer, struct timespec period) {
    time_add(*timer, period, timer);
    int e;
    do {
        e = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, timer, NULL);
    } while (e < 0 && errno == EINTR);
    if (e < 0) {
        spacepi_perror("clock_nanosleep", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    return 0;
}

void time_add(struct timespec a, struct timespec b, struct timespec *sum) {
    uint32_t nanos = ((uint32_t) a.tv_nsec) + (uint32_t) b.tv_nsec;
    sum->tv_sec = a.tv_sec + b.tv_sec + (nanos / 1000000000);
    sum->tv_nsec = nanos % 1000000000;
}

void time_sub(struct timespec a, struct timespec b, struct timespec *diff) {
    uint32_t nanos = (1000000000 + (uint32_t) a.tv_nsec) - (uint32_t) b.tv_nsec;
    diff->tv_sec = a.tv_sec - b.tv_sec - 1 + (nanos / 1000000000);
    diff->tv_nsec = nanos % 1000000000;
}
