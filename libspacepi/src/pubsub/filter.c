#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <spacepi-private.h>

int spacepi_pubsub_send_filtered(MQTTAsync handle, const char *topic_name, int payload_len, const void *payload, int qos, int retained, MQTTAsync_responseOptions *resp) {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static char *buf = NULL;
    static int buf_len = 0;
    int err = 1;
    CHECK_ERROR_JUMP(error, pthread_mutex_lock, &mutex);
    int needed_len = payload_len + sizeof(spacepi_pid_t);
    if (needed_len > buf_len) {
        if (buf) {
            free(buf);
        }
        CHECK_ALLOC_ARRAY_JUMP(error, buf, char, needed_len);
        *((spacepi_pid_t *) buf) = pubsub_state->pid;
    }
    memcpy(buf + sizeof(spacepi_pid_t), payload, payload_len);
    CHECK_ERROR_JUMP(error, MQTTAsync_send, handle, topic_name, needed_len, buf, qos, retained, resp);

    err = 0;
    error:
    CHECK_ERROR(pthread_mutex_unlock, &mutex);
    if (err) {
        RETURN_REPORTED_ERROR();
    }
    return 0;
}

int spacepi_pubsub_filter(spacepi_filter_t level) {
    if (level < sf_none || level > sf_same_pid) {
        RETURN_ERROR_SYSTEM(EINVAL);
    }
    pubsub_state->filter = level;
    return 0;
}

int spacepi_private_pubsub_message_arrived_filterer(void *context, char *topic_name, int topic_len, MQTTAsync_message *message) {
    if (message->payloadlen < sizeof(spacepi_pid_t)) {
        fflush(stdout);
        fputs("libspacepi: pubsub: malformed packet\n", stderr);
    }
    switch (pubsub_state->filter) {
        case sf_none:
            break;
        case sf_same_pid:
            if (*((spacepi_pid_t *) message->payload) == pubsub_state->pid) {
                return 0;
            }
            break;
        default:
            fflush(stdout);
            fputs("libspacepi: pubsub: invalid filter level\n", stderr);
            break;
    }
    MQTTAsync_message msg = *message;
    msg.payloadlen -= sizeof(spacepi_pid_t);
    msg.payload += sizeof(spacepi_pid_t);
    CHECK_ERROR(spacepi_private_pubsub_message_arrived, context, topic_name, topic_len, &msg);
    return 0;
}
