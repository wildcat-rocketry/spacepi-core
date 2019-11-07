#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <spacepi.h>

#define PUBLISH_CALLBACK_TABLE_BITS 4

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

struct _connection_callback_list;
typedef struct _connection_callback_list connection_callback_list_t;
struct _connection_callback_list {
    spacepi_connection_callback callback;
    void *context;
    connection_callback_list_t *next;
};

struct _publish_callback_list;
typedef struct _publish_callback_list publish_callback_list_t;
struct _publish_callback_list {
    spacepi_published_callback callback;
    void *context;
    char *channel;
    void *data;
    size_t data_len;
    spacepi_qos_t qos;
    int retain;
    int token;
    publish_callback_list_t *next;
};

struct _subscription_callback_list;
typedef struct _subscription_callback_list subscription_callback_list_t;
struct _subscription_callback_list {
    spacepi_subscription_callback callback;
    void *context;
    subscription_callback_list_t *next;
};
struct _subscription_callback_trie;
typedef struct _subscription_callback_trie subscription_callback_trie_t;
struct _subscription_callback_trie {
    subscription_callback_list_t *subscriptions;
    spacepi_qos_t qos;
    subscription_callback_trie_t *children[65]; // a-z A-Z 0-9 / + #
    subscription_callback_trie_t *parent;
};

static MQTTClient client;
int initialized = 0;
spacepi_pubsub_connection connection_state;
connection_callback_list_t *connection_callbacks;
publish_callback_list_t *publish_callbacks[1 << PUBLISH_CALLBACK_TABLE_BITS];
subscription_callback_trie_t *subscription_callbacks;

const signed char trie_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, 64, -1, -1, -1, -1, -1, -1, -1, 63, -1, -1, -1, 62,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};
#define TRIE_WILDCARD_ALL 64
#define TRIE_WILDCARD_SINGLE 63
#define TRIE_WILDCARD_END 62

void spacepi_pubsub_free_trie(subscription_callback_trie_t *trie);
void spacepi_pubsub_callback_connection_lost(void *context, char *cause);
int spacepi_pubsub_callback_message_arrived(void *context, char *channel, int channel_len, MQTTClient_message *message);
void spacepi_pubsub_callback_message_arrived_recursive(const char *channel, char *channel_it, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
void spacepi_pubsub_callback_message_arrived_call(const char *channel, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
void spacepi_pubsub_callback_delivery_complete(void *context, MQTTClient_deliveryToken token);
void spacepi_pubsub_callback_disconnected(void *context, MQTTProperties *properties, enum MQTTReasonCodes reason);
void spacepi_pubsub_callback_published(void *context, int token, int packet_type, MQTTProperties *properties, enum MQTTReasonCodes reason);

int spacepi_pubsub_init(void) {
    if (initialized) {
        return ~(errno = EISCONN);
    }
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
        spacepi_pubsub_cleanup();
        return ~(errno = e);
    }
    if ((e = MQTTClient_setCallbacks(client, NULL, spacepi_pubsub_callback_connection_lost, spacepi_pubsub_callback_message_arrived, spacepi_pubsub_callback_delivery_complete)) != MQTTCLIENT_SUCCESS) {
        spacepi_pubsub_cleanup();
        return ~(errno = e);
    }
    if ((e = MQTTClient_setDisconnected(client, NULL, spacepi_pubsub_callback_disconnected)) != MQTTCLIENT_SUCCESS) {
        spacepi_pubsub_cleanup();
        return ~(errno = e);
    }
    if ((e = MQTTClient_setPublished(client, NULL, spacepi_pubsub_callback_published)) != MQTTCLIENT_SUCCESS) {
        spacepi_pubsub_cleanup();
        return ~(errno = e);
    }
    initialized = 1;
    connection_state = connected;
    connection_callbacks = NULL;
    for (int i = (1 << PUBLISH_CALLBACK_TABLE_BITS) - 1; i >= 0; --i) {
        publish_callbacks[i] = NULL;
    }
    subscription_callbacks = NULL;
    return 0;
}

int spacepi_pubsub_cleanup(void) {
    initialized = 0;
    int e;
    if ((e = MQTTClient_disconnect(client, MQTT_DISCONNECT_TIMEOUT)) == MQTTCLIENT_SUCCESS) {
        e = 0;
    }
    MQTTClient_destroy(&client);
    if (e) {
        errno = ~e;
    }
    while (connection_callbacks) {
        connection_callback_list_t *tmp = connection_callbacks->next;
        free(connection_callbacks);
        connection_callbacks = tmp;
    }
    for (int i = (1 << PUBLISH_CALLBACK_TABLE_BITS) - 1; i >= 0; --i) {
        publish_callback_list_t *list = publish_callbacks[i];
        publish_callbacks[i] = NULL;
        while (list) {
            publish_callback_list_t *tmp = list;
            free(list);
            list = tmp;
        }
    }
    if (subscription_callbacks) {
        spacepi_pubsub_free_trie(subscription_callbacks);
    }
    return e;
}

spacepi_pubsub_connection spacepi_pubsub_connected(void) {
    return connection_state;
}

int spacepi_pubsub_connection_handler(spacepi_connection_callback callback, void *context) {
    connection_callback_list_t *node = (connection_callback_list_t *) malloc(sizeof(connection_callback_list_t));
    if (!node) {
        return ~(errno = ENOBUFS);
    }
    node->callback = callback;
    node->context = context;
    node->next = connection_callbacks;
    connection_callbacks = node;
    return 0;
}

int spacepi_publish(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    int e;
    if ((e = MQTTClient_publish(client, channel, data_len, data, qos, retain, NULL)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    return 0;
}

int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token) {
    int e;
    if ((e = MQTTClient_publish(client, channel, data_len, data, qos, retain, token)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    return 0;
}

int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback, void *context) {
    publish_callback_list_t *node = (publish_callback_list_t *) malloc(sizeof(publish_callback_list_t));
    if (!node) {
        return ~(errno = ENOBUFS);
    }
    node->callback = callback;
    node->context = context;
    int channel_len = strlen(channel);
    node->channel = (char *) malloc(channel_len + 1);
    if (!node->channel) {
        free(node);
        return ~(errno = ENOBUFS);
    }
    strcpy(node->channel, channel);
    if (data_len != 0) {
        node->data = malloc(data_len);
        if (!node->data) {
            free(node->channel);
            free(node);
            return ~(errno = ENOBUFS);
        }
        memcpy(node->data, data, data_len);
    } else {
        node->data = NULL;
    }
    node->data_len = data_len;
    node->qos = qos;
    node->retain = retain;
    int e;
    if ((e = MQTTClient_publish(client, channel, data_len, data, qos, retain, &node->token)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    int fanout = (node->token & (-1 ^ (-1 << PUBLISH_CALLBACK_TABLE_BITS)));
    node->next = publish_callbacks[fanout];
    publish_callbacks[fanout] = node;
    return 0;
}

int spacepi_wait_token(spacepi_token_t token, unsigned long timeout) {
    int e;
    if ((e = MQTTClient_waitForCompletion(client, token, timeout)) != MQTTCLIENT_SUCCESS) {
        return ~(errno = e);
    }
    return 0;
}

int spacepi_subscribe(const char *channel, spacepi_qos_t qos, spacepi_subscription_callback callback, void *context) {
    const unsigned char *channel_it = (const unsigned char *) channel;
    subscription_callback_trie_t **trie_it = &subscription_callbacks;
    while (*channel_it && *trie_it) {
        int c = trie_table[*channel_it++];
        if (c < 0) {
            return ~(errno = EINVAL);
        }
        trie_it = &(*trie_it)->children[c];
    }
    subscription_callback_trie_t **alloc_start = *channel_it ? trie_it : NULL;
    while (*channel_it) {
        int c = trie_table[*channel_it++];
        if (c < 0) {
            return ~(errno = EINVAL);
        }
        subscription_callback_trie_t *tmp = *trie_it;
        *trie_it = (subscription_callback_trie_t *) malloc(sizeof(subscription_callback_trie_t));
        if (!*trie_it) {
            if (*alloc_start) {
                spacepi_pubsub_free_trie(*alloc_start);
                *alloc_start = NULL;
            }
            return ~(errno = ENOBUFS);
        }
        (*trie_it)->subscriptions = NULL;
        for (int i = 64; i >= 0; --i) {
            (*trie_it)->children[i] = NULL;
        }
        (*trie_it)->parent = tmp;
        trie_it = &(*trie_it)->children[c];
    }
    subscription_callback_list_t *node = (subscription_callback_list_t *) malloc(sizeof(subscription_callback_list_t));
    if (!node) {
        if (alloc_start) {
            if (*alloc_start) {
                spacepi_pubsub_free_trie(*alloc_start);
                *alloc_start = NULL;
            }
        }
        return ~(errno = ENOBUFS);
    }
    node->callback = callback;
    node->context = context;
    node->next = (*trie_it)->subscriptions;
    (*trie_it)->subscriptions = node;
    if (!node->next) {
        int e;
        if ((e = MQTTClient_subscribe(client, channel, qos)) != MQTTCLIENT_SUCCESS) {
            (*trie_it)->subscriptions = NULL;
            if (alloc_start) {
                if (*alloc_start) {
                    spacepi_pubsub_free_trie(*alloc_start);
                    *alloc_start = NULL;
                }
            }
            free(node);
            return ~(errno = e);
        }
        (*trie_it)->qos = qos;
    } else if ((*trie_it)->qos != qos && (*trie_it)->qos != exactly_once) {
        int e;
        if ((e = MQTTClient_subscribe(client, channel, exactly_once)) != MQTTCLIENT_SUCCESS) {
            (*trie_it)->subscriptions = node->next;
            free(node);
            return ~(errno = e);
        }
        (*trie_it)->qos = exactly_once;
    }
    return 0;
}

int spacepi_unsubscribe(const char *channel, spacepi_subscription_callback callback, void *context) {
    const unsigned char *channel_it = (const unsigned char *) channel;
    subscription_callback_trie_t *trie_it = subscription_callbacks;
    while (*channel_it && trie_it) {
        int c = trie_table[*channel_it++];
        if (c < 0) {
            return ~(errno = EINVAL);
        }
        trie_it = trie_it->children[c];
    }
    if (*channel_it) {
        return ~(errno = ENOENT);
    }
    subscription_callback_list_t list;
    list.next = trie_it->subscriptions;
    subscription_callback_list_t *list_it = &list;
    while (list_it->next) {
        if ((callback == NULL || list_it->next->callback == callback) && (context == NULL || list_it->next->context == context)) {
            subscription_callback_list_t *tmp = list_it;
            list_it->next = list_it->next->next;
            free(tmp);
        }
    }
    trie_it->subscriptions = list.next;
    int e = 0;
    if (!trie_it->subscriptions) {
        if ((e = MQTTClient_unsubscribe(client, channel)) == MQTTCLIENT_SUCCESS) {
            e = 0;
        }
    }
    while (trie_it) {
        if (trie_it->subscriptions) {
            break;
        }
        int remove = 1;
        for (int i = 64; i >= 0; --i) {
            if (trie_it->children[i]) {
                remove = 0;
                break;
            }
        }
        if (remove) {
            subscription_callback_trie_t *tmp = trie_it;
            trie_it = trie_it->parent;
            free(tmp);
        } else {
            break;
        }
    }
    if (e) {
        return ~(errno = e);
    } else {
        return 0;
    }
}

void spacepi_pubsub_free_trie(subscription_callback_trie_t *trie) {
    for (int i = 64; i >= 0; ++i) {
        if (trie->children[i]) {
            spacepi_pubsub_free_trie(trie->children[i]);
        }
    }
    subscription_callback_list_t *list = trie->subscriptions;
    while (list) {
        subscription_callback_list_t *tmp = list->next;
        free(tmp);
        list = tmp;
    }
    free(trie);
}

void spacepi_pubsub_callback_connection_lost(void *context, char *cause) {
    connection_state = disconnected;
    // TODO start reconnecting
    for (connection_callback_list_t *it = connection_callbacks; it; it = it->next) {
        it->callback(it->context, connection_state);
    }
}

int spacepi_pubsub_callback_message_arrived(void *context, char *channel, int channel_len, MQTTClient_message *message) {
    spacepi_pubsub_callback_message_arrived_recursive(channel, channel, subscription_callbacks, message->payload, message->payloadlen, message->qos, message->retained);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(channel);
    return TRUE;
}

void spacepi_pubsub_callback_message_arrived_recursive(const char *channel, char *channel_it, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    while (*channel_it && trie_it) {
        if (trie_it->children[TRIE_WILDCARD_ALL]) {
            spacepi_pubsub_callback_message_arrived_call(channel, trie_it->children[TRIE_WILDCARD_ALL], data, data_len, qos, retain);
        }
        if (trie_it->children[TRIE_WILDCARD_SINGLE]) {
            char *it = channel_it;
            while (*it && *it != '/') {
                ++it;
            }
            if (*it) {
                spacepi_pubsub_callback_message_arrived_recursive(channel, it + 1, trie_it->children[TRIE_WILDCARD_SINGLE], data, data_len, qos, retain);
            } else {
                spacepi_pubsub_callback_message_arrived_call(channel, trie_it->children[TRIE_WILDCARD_ALL], data, data_len, qos, retain);
            }
        }
        int c = trie_table[*channel_it++];
        if (c < 0) {
            return;
        }
        trie_it = trie_it->children[c];
    }
    if (*channel_it) {
        return;
    }
    spacepi_pubsub_callback_message_arrived_call(channel, trie_it, data, data_len, qos, retain);
}

void spacepi_pubsub_callback_message_arrived_call(const char *channel, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    for (subscription_callback_list_t *it = trie_it->subscriptions; it; it = it->next) {
        it->callback(it->context, channel, data, data_len, qos, retain);
    }
}

void spacepi_pubsub_callback_delivery_complete(void *context, MQTTClient_deliveryToken token) {
    int fanout = (token & (-1 ^ (-1 << PUBLISH_CALLBACK_TABLE_BITS)));
    publish_callback_list_t list;
    list.next = publish_callbacks[fanout];
    for (publish_callback_list_t *it = &list; it->next; it = it->next) {
        if (it->next->token == token) {
            publish_callback_list_t *tmp = it->next;
            it->next = it->next->next;
            tmp->callback(tmp->context, tmp->channel, tmp->data, tmp->data_len, tmp->qos, tmp->retain);
            free(tmp->channel);
            free(tmp->data);
            free(tmp);
            break;
        }
    }
    publish_callbacks[fanout] = list.next;
}

void spacepi_pubsub_callback_disconnected(void *context, MQTTProperties *properties, enum MQTTReasonCodes reason) {
    MQTTClient_disconnect(client, 1);
}

void spacepi_pubsub_callback_published(void *context, int token, int packet_type, MQTTProperties *properties, enum MQTTReasonCodes reason) {
    spacepi_pubsub_callback_delivery_complete(context, token);
}
