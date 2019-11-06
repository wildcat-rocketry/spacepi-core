#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>
#include <spacepi.h>

#ifndef MQTT_HOST
#define MQTT_HOST tcp://localhost:1883
#endif
#ifndef MQTT_KEEP_ALIVE
#define MQTT_KEEP_ALIVE 2
#endif
#ifndef MQTT_CONNECT_TIMEOUT
#define MQTT_CONNECT_TIMEOUT 5
#endif
#ifndef MQTT_DISCONNECT_TIMEOUT
#define MQTT_DISCONNECT_TIMEOUT 2000
#endif

static MQTTClient client;

int spacepi_pubsub_init(void) {
    char client_id[23];
    strcpy(client_id, "spacepi");
    randomize_string(base_62, 1, 62, client_id + 7, 23 - 7);
    client_id[22] = 0;
    int e;
    if ((e = MQTTClient_create(&client, STR(MQTT_HOST), client_id, MQTTCLIENT_PERSISTENCE_DEFAULT, NULL)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    MQTTClient_connectOptions conn_opts = {
        .keepAliveInterval = MQTT_KEEP_ALIVE,
        .cleansession = TRUE,
        .will = NULL,
        .username = NULL,
        .password = NULL,
        .connectTimeout = MQTT_CONNECT_TIMEOUT,
        .retryInterval = 0,
        .ssl = NULL,
        .serverURIcount = 0,
        .MQTTVersion = MQTTVERSION_DEFAULT
    };
    if ((e = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    return 0;
}

int spacepi_pubsub_cleanup(void) {
    int e;
    if ((e = MQTTClient_disconnect(client, MQTT_DISCONNECT_TIMEOUT)) == MQTTCLIENT_SUCCESS) {
        e = 0;
    }
    MQTTClient_destroy(&client);
    if (e) {
        errno = ~e;
    }
    return e;
}

int spacepi_pubsub_connected(void) {
    return ~(errno = ENOSYS);
}

int spacepi_pubsub_connection_handler(spacepi_connection_callback callback) {
    return ~(errno = ENOSYS);
}

int spacepi_publish(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    int e;
    if ((e = MQTTClient_publish(client, channel, data_len, data, qos, retain, NULL)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    return 0;
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
