#include <errno.h>
#include <stdio.h>
#include <spacepi.h>

int spacepi_pubsub_init(void) {
    return ~(errno = ENOSYS);
}

int spacepi_pubsub_cleanup(void) {
    return ~(errno = ENOSYS);
}

int spacepi_pubsub_connected(void) {
    return ~(errno = ENOSYS);
}

int spacepi_pubsub_connection_handler(spacepi_connection_callback callback) {
    return ~(errno = ENOSYS);
}

int spacepi_publish(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    return ~(errno = ENOSYS);
}

int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token) {
    return ~(errno = ENOSYS);
}

int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback) {
    return ~(errno = ENOSYS);
}

int spacepi_wait_token(spacepi_token_t token, time_t timeout) {
    return ~(errno = ENOSYS);
}

int spacepi_subscribe(const char *channel, spacepi_qos_t qos, spacepi_subscription_callback callback) {
    return ~(errno = ENOSYS);
}

int spacepi_unsubscribe(const char *channel, spacepi_subscription_callback callback) {
    return ~(errno = ENOSYS);
}
