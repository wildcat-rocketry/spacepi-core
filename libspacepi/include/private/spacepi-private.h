#ifndef SPACEPI_PRIVATE_H
#define SPACEPI_PRIVATE_H

#include <pthread.h>
#include <MQTTAsync.h>
#include <spacepi.h>

/* Error handling framework */

#define SET_ERROR_SPACEPI(val) SET_ERROR(spacepi, val)
#define RETURN_ERROR_SPACEPI(val) RETURN_ERROR(spacepi, val)
#define JUMP_ERROR_SPACEPI(label, val) JUMP_ERROR(label, spacepi, val)
#define SET_ERROR_MQTT(val) SET_ERROR(mqtt, ~val)
#define RETURN_ERROR_MQTT(val) RETURN_ERROR(mqtt, val)
#define CHECK_ERROR_MQTT(func, ...) do { int __err = func(__VA_ARGS__); if (__err != MQTTASYNC_SUCCESS) { SET_ERROR_MQTT(__err); spacepi_perror(STR(func), __FILE__, __LINE__); RETURN_REPORTED_ERROR(); } } while (0)
#define JUMP_ERROR_MQTT(label, val) JUMP_ERROR(label, mqtt, val)
#define CHECK_ERROR_JUMP_MQTT(label, func, ...) do { int __err = func(__VA_ARGS__); if (__err != MQTTASYNC_SUCCESS) { SET_ERROR_MQTT(__err); spacepi_perror(STR(func), __FILE__, __LINE__); goto label; } } while (0)

/* Pub/sub framework */

typedef struct _pubsub_connection_handler_list pubsub_connection_handler_list_t;
struct _pubsub_connection_handler_list {
    spacepi_connection_callback callback;
    void *context;
    pubsub_connection_handler_list_t *next;
};

typedef struct _pubsub_subscription_list pubsub_subscription_list_t;
struct _pubsub_subscription_list {
    spacepi_subscription_callback callback;
    void *context;
    pubsub_subscription_list_t *next;
};

typedef struct _pubsub_subscription_tree pubsub_subscription_tree_t;
struct _pubsub_subscription_tree {
    const char *channel;
    spacepi_qos_t qos;
    int depth;
    int children;
    pubsub_subscription_list_t *list;
    pubsub_subscription_tree_t *less;
    pubsub_subscription_tree_t *greater;
};

typedef struct {
    MQTTAsync mqtt;
    spacepi_pubsub_connection_t conn;
    pthread_mutex_t mutex;
    pubsub_connection_handler_list_t *connection_handlers;
    spacepi_server_down_callback server_down_callback;
    void *server_down_context;
    pubsub_subscription_tree_t *subscriptions;
} pubsub_state_t;

extern pubsub_state_t *pubsub_state;

int spacepi_private_pubsub_connect(pubsub_state_t *state, int clean);

// Callbacks
void spacepi_private_pubsub_connection_lost(void *context, char *cause);
int spacepi_private_pubsub_message_arrived(void *context, char *topic_name, int topic_len, MQTTAsync_message *message);

#endif
