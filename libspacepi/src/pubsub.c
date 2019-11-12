#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MQTTAsync.h>
#include <spacepi.h>

#ifdef MQTT_HOST
#define MQTT_HOST_STR STR(MQTT_HOST)
#else
#define MQTT_HOST_STR "tcp://localhost:1883"
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
#ifdef MQTT_PERSISTENCE_DIR
#define MQTT_PERSISTENCE_DIR /tmp
#endif
#ifndef MQTT_RETRY_INTERVAL_MIN
#define MQTT_RETRY_INTERVAL_MIN 1
#endif
#ifndef MQTT_RETRY_INTERVAL_MAX
#define MQTT_RETRY_INTERVAL_MAX 64
#endif

struct _connection_callback_list;
typedef struct _connection_callback_list connection_callback_list_t;
struct _connection_callback_list {
    spacepi_connection_callback callback;
    void *context;
    connection_callback_list_t *next;
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
    subscription_callback_trie_t *children[40]; // a-z 0-9 / + # -
    subscription_callback_trie_t *parent;
};

typedef struct {
    const char *message;
    spacepi_published_callback callback;
    void *context;
    const char *channel;
    void *data;
    size_t data_len;
    int qos;
    int retain;
} destroy_context_t;

static MQTTAsync client;
static int globally_initialized = 0;
static int initialized = 0;
static spacepi_pubsub_connection_t connection_state;
static connection_callback_list_t *connection_callbacks;
static subscription_callback_trie_t *subscription_callbacks;

const static signed char const trie_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, 38, -1, -1, -1, -1, -1, -1, -1, 37, -1, 39, -1, 36,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
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
#define TRIE_WILDCARD_ALL 38
#define TRIE_WILDCARD_SINGLE 37
#define TRIE_WILDCARD_END 36

static void free_trie(subscription_callback_trie_t *trie);
static void callback_connection_lost(void *context, const char *cause);
static int callback_message_arrived(void *context, char *channel, int channel_len, MQTTAsync_message *message);
static void callback_message_arrived_recursive(const char *channel, char *channel_it, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
static void callback_message_arrived_call(const char *channel, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
static void callback_disconnected(void *context, MQTTProperties *properties, enum MQTTReasonCodes reason);
static void failure_printing_callback(void *context, MQTTAsync_failureData *res);
static void failure_printing_callback5(void *context, MQTTAsync_failureData5 *res);
static void callback_connected(void *context, char *cause);
static void destroy_after_disconnect_s(void *context, MQTTAsync_successData *res);
static void destroy_after_disconnect_s5(void *context, MQTTAsync_successData5 *res);
static void destroy_after_disconnect_f(void *context, MQTTAsync_failureData *res);
static void destroy_after_disconnect_f5(void *context, MQTTAsync_failureData5 *res);
static void publish_complete_callback(void *context, MQTTAsync_successData *res);
static void publish_complete_callback5(void *context, MQTTAsync_successData5 *res);

int spacepi_pubsub_init(void) {
    if (!globally_initialized) {
        MQTTAsync_init_options opts = {
            .struct_id = { 'M', 'Q', 'T', 'G' },
            .struct_version = 0,
            .do_openssl_init = FALSE
        };
        MQTTAsync_global_init(&opts);
    }
    if (initialized) {
        return ~(errno = EISCONN);
    }
    char client_id[23];
    strcpy(client_id, "spacepi");
    random_reseed();
    randomize_string(base_62, 1, 62, client_id + 7, 23 - 7);
    client_id[22] = 0;
    int e;
    if ((e = MQTTAsync_create(&client, MQTT_HOST_STR, client_id, MQTTCLIENT_PERSISTENCE_DEFAULT, STR(MQTT_PERSISTENCE_DIR))) != MQTTASYNC_SUCCESS) {
        return -(errno = -e);
    }
    destroy_context_t des = {
        .message = "Failed to connect to MQTT server.",
        .data = NULL
    };
    MQTTAsync_connectOptions conn_opts = {
        .struct_id = { 'M', 'Q', 'T', 'C' },
        .struct_version = 6,
        .keepAliveInterval = MQTT_KEEP_ALIVE,
        .cleansession = TRUE,
        .maxInflight = -1,
        .will = NULL,
        .username = NULL,
        .password = NULL,
        .connectTimeout = MQTT_CONNECT_TIMEOUT,
        .retryInterval = 0,
        .ssl = NULL,
        .onSuccess = NULL,
        .onFailure = failure_printing_callback,
        .context = &des,
        .serverURIcount = 0,
        .serverURIs = NULL,
        .MQTTVersion = MQTTVERSION_DEFAULT,
        .automaticReconnect = TRUE,
        .minRetryInterval = MQTT_RETRY_INTERVAL_MIN,
        .maxRetryInterval = MQTT_RETRY_INTERVAL_MAX,
        .binarypwd = {
            .len = 0,
            .data = NULL
        },
        .cleanstart = FALSE, // MQTTv5 only
        .connectProperties = NULL,
        .willProperties = NULL,
        .onSuccess5 = NULL,
        .onFailure5 = NULL // MQTTv5 only - failure_printing_callback5
    };
    connection_state = disconnected;
    if ((e = MQTTAsync_setCallbacks(client, NULL, (void (*)(void *, char *)) callback_connection_lost, callback_message_arrived, NULL)) != MQTTASYNC_SUCCESS) {
        spacepi_pubsub_cleanup();
        return -(errno = -e);
    }
    if ((e = MQTTAsync_setDisconnected(client, NULL, callback_disconnected)) != MQTTASYNC_SUCCESS) {
        spacepi_pubsub_cleanup();
        return -(errno = -e);
    }
    if ((e = MQTTAsync_setConnected(client, NULL, callback_connected)) != MQTTASYNC_SUCCESS) {
        spacepi_pubsub_cleanup();
        return -(errno = -e);
    }
    if ((e = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        spacepi_pubsub_cleanup();
        return -(errno = -e);
    }
    initialized = TRUE;
    connection_callbacks = NULL;
    subscription_callbacks = NULL;
    return 0;
    // TODO Allow functions to queue up when disconnected
}

int spacepi_pubsub_cleanup(void) {
    initialized = FALSE;
    int e;
    MQTTAsync_disconnectOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'D' },
        .struct_version = 1,
        .timeout = MQTT_DISCONNECT_TIMEOUT,
        .onSuccess = destroy_after_disconnect_s,
        .onFailure = destroy_after_disconnect_f,
        .context = client,
        .properties = {
            .count = 0,
            .max_count = 0,
            .length = 0,
            .array = NULL
        },
        .reasonCode = MQTTREASONCODE_NORMAL_DISCONNECTION,
        .onSuccess5 = NULL, // MQTTv5 only - destroy_after_disconnect_s5,
        .onFailure5 = NULL // MQTTv5 only - destroy_after_disconnect_f5
    };
    if ((e = MQTTAsync_disconnect(client, &opts)) == MQTTASYNC_SUCCESS) {
        e = 0;
    }
    if (e == MQTTASYNC_DISCONNECTED) {
        e = 0;
    }
    client = NULL;
    while (connection_callbacks) {
        connection_callback_list_t *tmp = connection_callbacks->next;
        free(connection_callbacks);
        connection_callbacks = tmp;
    }
    if (subscription_callbacks) {
        free_trie(subscription_callbacks);
    }
    if (e) {
        return -(errno = -e);
    }
    return 0;
}

spacepi_pubsub_connection_t spacepi_pubsub_connected(void) {
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
    destroy_context_t des = {
        .message = "Failed to publish message.",
        .data = NULL
    };
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = failure_printing_callback,
        .context = &des,
        .token = 0,
        .onSuccess5 = NULL,
        .onFailure5 = NULL, // MQTTv5 only - failure_printing_callback5,
        .properties = {
            .count = 0,
            .max_count = 0,
            .length = 0,
            .array = NULL
        },
        .subscribeOptions = {
            .struct_id = { 'M', 'Q', 'T', 'O' },
            .struct_version = 0,
            .noLocal = 0,
            .retainAsPublished = 0,
            .retainHandling = 0
        },
        .subscribeOptionsCount = 0,
        .subscribeOptionsList = NULL
    };
    if ((e = MQTTAsync_send(client, channel, data_len, data, qos, retain, NULL)) != MQTTASYNC_SUCCESS) {
        return -(errno = -e);
    }
    return 0;
}

int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token) {
    int e;
    destroy_context_t des = {
        .message = "Failed to publish message.",
        .data = NULL
    };
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = failure_printing_callback,
        .context = &des,
        .token = 0,
        .onSuccess5 = NULL,
        .onFailure5 = NULL, // MQTTv5 only - failure_printing_callback5,
        .properties = {
            .count = 0,
            .max_count = 0,
            .length = 0,
            .array = NULL
        },
        .subscribeOptions = {
            .struct_id = { 'M', 'Q', 'T', 'O' },
            .struct_version = 0,
            .noLocal = 0,
            .retainAsPublished = 0,
            .retainHandling = 0
        },
        .subscribeOptionsCount = 0,
        .subscribeOptionsList = NULL
    };
    if ((e = MQTTAsync_send(client, channel, data_len, data, qos, retain, &opts)) != MQTTASYNC_SUCCESS) {
        return -(errno = -e);
    }
    *token = opts.token;
    return 0;
}

int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback, void *context) {
    void *data_clone = malloc(data_len);
    if (!data_clone) {
        return ~(errno = ENOBUFS);
    }
    memcpy(data_clone, data, data_len);
    int e;
    destroy_context_t des = {
        .message = "Failed to publish message.",
        .callback = callback,
        .context = context,
        .channel = channel,
        .data = data_clone,
        .data_len = data_len,
        .qos = qos,
        .retain = retain
    };
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = failure_printing_callback,
        .context = &des,
        .token = 0,
        .onSuccess5 = NULL,
        .onFailure5 = NULL, // MQTTv5 only - failure_printing_callback5,
        .properties = {
            .count = 0,
            .max_count = 0,
            .length = 0,
            .array = NULL
        },
        .subscribeOptions = {
            .struct_id = { 'M', 'Q', 'T', 'O' },
            .struct_version = 0,
            .noLocal = 0,
            .retainAsPublished = 0,
            .retainHandling = 0
        },
        .subscribeOptionsCount = 0,
        .subscribeOptionsList = NULL
    };
    if ((e = MQTTAsync_send(client, channel, data_len, data, qos, retain, &opts)) != MQTTASYNC_SUCCESS) {
        return -(errno = -e);
    }
    return 0;
}

int spacepi_wait_token(spacepi_token_t token, unsigned long timeout) {
    int e;
    if ((e = MQTTAsync_waitForCompletion(client, token, timeout)) != MQTTASYNC_SUCCESS) {
        return -(errno = -e);
    }
    return 0;
}

int spacepi_subscribe(const char *channel, spacepi_qos_t qos, spacepi_subscription_callback callback, void *context) {
    const unsigned char *channel_it = (const unsigned char *) channel;
    subscription_callback_trie_t *trie_last = NULL;
    subscription_callback_trie_t **trie_it = &subscription_callbacks;
    while (*channel_it && *trie_it) {
        int c = trie_table[*channel_it++];
        if (c < 0) {
            return ~(errno = EINVAL);
        }
        trie_last = *trie_it;
        trie_it = &(*trie_it)->children[c];
    }
    subscription_callback_trie_t **alloc_start = *channel_it ? trie_it : NULL;
    while (*channel_it) {
        int c = trie_table[*channel_it++];
        if (c < 0) {
            return ~(errno = EINVAL);
        }
        *trie_it = (subscription_callback_trie_t *) malloc(sizeof(subscription_callback_trie_t));
        if (!*trie_it) {
            if (*alloc_start) {
                free_trie(*alloc_start);
                *alloc_start = NULL;
            }
            return ~(errno = ENOBUFS);
        }
        (*trie_it)->subscriptions = NULL;
        for (int i = 39; i >= 0; --i) {
            (*trie_it)->children[i] = NULL;
        }
        (*trie_it)->parent = trie_last;
        trie_last = *trie_it;
        trie_it = &(*trie_it)->children[c];
    }
    if (!*trie_it) {
        *trie_it = (subscription_callback_trie_t *) malloc(sizeof(subscription_callback_trie_t));
        if (!*trie_it) {
            if (*alloc_start) {
                free_trie(*alloc_start);
                *alloc_start = NULL;
            }
            return ~(errno = ENOBUFS);
        }
        (*trie_it)->subscriptions = NULL;
        for (int i = 39; i >= 0; --i) {
            (*trie_it)->children[i] = NULL;
        }
        (*trie_it)->parent = trie_last;
    }
    subscription_callback_list_t *node = (subscription_callback_list_t *) malloc(sizeof(subscription_callback_list_t));
    if (!node) {
        if (alloc_start) {
            if (*alloc_start) {
                free_trie(*alloc_start);
                *alloc_start = NULL;
            }
        }
        return ~(errno = ENOBUFS);
    }
    node->callback = callback;
    node->context = context;
    node->next = (*trie_it)->subscriptions;
    (*trie_it)->subscriptions = node;
    destroy_context_t des = {
        .message = "Failed to register subscription.",
        .data = NULL
    };
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = failure_printing_callback,
        .context = &des,
        .token = 0,
        .onSuccess5 = NULL,
        .onFailure5 = NULL, // MQTTv5 only - failure_printing_callback5,
        .properties = {
            .count = 0,
            .max_count = 0,
            .length = 0,
            .array = NULL
        },
        .subscribeOptions = {
            .struct_id = { 'M', 'Q', 'T', 'O' },
            .struct_version = 0,
            .noLocal = 0,
            .retainAsPublished = 0,
            .retainHandling = 0
        },
        .subscribeOptionsCount = 0,
        .subscribeOptionsList = NULL
    };
    if (!node->next) {
        int e;
        if ((e = MQTTAsync_subscribe(client, channel, qos, &opts)) != MQTTASYNC_SUCCESS) {
            (*trie_it)->subscriptions = NULL;
            if (alloc_start) {
                if (*alloc_start) {
                    free_trie(*alloc_start);
                    *alloc_start = NULL;
                }
            }
            free(node);
            return -(errno = -e);
        }
        (*trie_it)->qos = qos;
    } else if ((*trie_it)->qos != qos && (*trie_it)->qos != exactly_once) {
        int e;
        if ((e = MQTTAsync_subscribe(client, channel, exactly_once, &opts)) != MQTTASYNC_SUCCESS) {
            (*trie_it)->subscriptions = node->next;
            free(node);
            return -(errno = -e);
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
            subscription_callback_list_t *tmp = list_it->next;
            list_it->next = list_it->next->next;
            free(tmp);
        } else {
            list_it = list_it->next;
        }
    }
    trie_it->subscriptions = list.next;
    int e = 0;
    if (!trie_it->subscriptions) {
        destroy_context_t des = {
            .message = "Failed to unsubscribe from channel.",
            .data = NULL
        };
        MQTTAsync_responseOptions opts = {
            .struct_id = { 'M', 'Q', 'T', 'R' },
            .struct_version = 1,
            .onSuccess = NULL,
            .onFailure = failure_printing_callback,
            .context = &des,
            .token = 0,
            .onSuccess5 = NULL,
            .onFailure5 = NULL, // MQTTv5 only - failure_printing_callback5,
            .properties = {
                .count = 0,
                .max_count = 0,
                .length = 0,
                .array = NULL
            },
            .subscribeOptions = {
                .struct_id = { 'M', 'Q', 'T', 'O' },
                .struct_version = 0,
                .noLocal = 0,
                .retainAsPublished = 0,
                .retainHandling = 0
            },
            .subscribeOptionsCount = 0,
            .subscribeOptionsList = NULL
        };
        if ((e = MQTTAsync_unsubscribe(client, channel, &opts)) == MQTTASYNC_SUCCESS) {
            e = 0;
        }
    }
    subscription_callback_trie_t *last_trie = NULL;
    while (trie_it) {
        if (trie_it->subscriptions) {
            if (last_trie) {
                for (int i = 39; i >= 0; --i) {
                    if (trie_it->children[i] == last_trie) {
                        trie_it->children[i] = NULL;
                    }
                }
            }
            break;
        }
        int remove = 1;
        for (int i = 39; i >= 0; --i) {
            if (trie_it->children[i] == last_trie) {
                trie_it->children[i] = NULL;
                if (!remove) {
                    break;
                }
            } else if (trie_it->children[i]) {
                remove = 0;
                if (!last_trie) {
                    break;
                }
            }
        }
        if (remove) {
            last_trie = trie_it;
            trie_it = trie_it->parent;
            free(last_trie);
            if (last_trie == subscription_callbacks) {
                subscription_callbacks = NULL;
                break;
            }
        } else {
            break;
        }
    }
    if (e) {
        return -(errno = -e);
    } else {
        return 0;
    }
}

static void free_trie(subscription_callback_trie_t *trie) {
    for (int i = 39; i >= 0; --i) {
        if (trie->children[i]) {
            free_trie(trie->children[i]);
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

static void callback_connection_lost(void *context, const char *cause) {
    if (connection_state == connected) {
        connection_state = disconnected;
        for (connection_callback_list_t *it = connection_callbacks; it; it = it->next) {
            it->callback(it->context, connection_state);
        }
        if (cause) {
            fprintf(stderr, "Lost connection from MQTT Server: %s\n", cause);
        } else {
            fputs("Lost connection from MQTT server.\n", stderr);
        }
    }
}

static int callback_message_arrived(void *context, char *channel, int channel_len, MQTTAsync_message *message) {
    callback_message_arrived_recursive(channel, channel, subscription_callbacks, message->payload, message->payloadlen, message->qos, message->retained);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(channel);
    return TRUE;
}

static void callback_message_arrived_recursive(const char *channel, char *channel_it, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    while (*channel_it && trie_it) {
        if (trie_it->children[TRIE_WILDCARD_ALL]) {
            callback_message_arrived_call(channel, trie_it->children[TRIE_WILDCARD_ALL], data, data_len, qos, retain);
        }
        if (trie_it->children[TRIE_WILDCARD_SINGLE]) {
            char *it = channel_it;
            while (*it && *it != '/') {
                ++it;
            }
            if (*it) {
                callback_message_arrived_recursive(channel, it + 1, trie_it->children[TRIE_WILDCARD_SINGLE], data, data_len, qos, retain);
            } else {
                callback_message_arrived_call(channel, trie_it->children[TRIE_WILDCARD_ALL], data, data_len, qos, retain);
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
    callback_message_arrived_call(channel, trie_it, data, data_len, qos, retain);
}

static void callback_message_arrived_call(const char *channel, subscription_callback_trie_t *trie_it, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    for (subscription_callback_list_t *it = trie_it->subscriptions; it; it = it->next) {
        it->callback(it->context, channel, data, data_len, qos, retain);
    }
}

static void callback_disconnected(void *context, MQTTProperties *properties, enum MQTTReasonCodes reason) {
    MQTTAsync_disconnect(client, NULL);
    callback_connection_lost(NULL, MQTTReasonCode_toString(reason));
}

static void failure_printing_callback(void *context, MQTTAsync_failureData *res) {
    destroy_context_t *message = (destroy_context_t *) context;
    fputs(message->message, stderr);
    if (message->data) {
        free(message->data);
    }
}

static void failure_printing_callback5(void *context, MQTTAsync_failureData5 *res) {
    failure_printing_callback(context, NULL);
}

static void callback_connected(void *context, char *cause) {
    connection_state = connected;
}

static void destroy_after_disconnect_s(void *context, MQTTAsync_successData *res) {
    MQTTAsync_destroy((MQTTAsync *) context);
}

static void destroy_after_disconnect_s5(void *context, MQTTAsync_successData5 *res) {
    destroy_after_disconnect_s(context, NULL);
}

static void destroy_after_disconnect_f(void *context, MQTTAsync_failureData *res) {
    fputs("Failed to disconnect socket.", stderr);
    destroy_after_disconnect_s(context, NULL);
}

static void destroy_after_disconnect_f5(void *context, MQTTAsync_failureData5 *res) {
    destroy_after_disconnect_f(context, NULL);
}

static void publish_complete_callback(void *context, MQTTAsync_successData *res) {
    destroy_context_t *message = (destroy_context_t *) context;
    message->callback(message->context, message->channel, message->data, message->data_len, message->qos, message->retain);
    free(message->data);
}

static void publish_complete_callback5(void *context, MQTTAsync_successData5 *res) {
    publish_complete_callback(context, NULL);
}
