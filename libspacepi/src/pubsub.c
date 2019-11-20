#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
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

struct _delayed_call;
typedef struct _delayed_call delayed_call_t;
struct _delayed_call {
    enum {
        send,
        subscribe,
        unsubscribe
    } type;
    char *channel;
    MQTTAsync_responseOptions response;
    MQTTAsync_responseOptions *response_ptr;
    union {
        struct {
            int payloadlen;
            void *payload;
            int qos;
            int retained;
            spacepi_token_t *token;
        } send;
        struct {
            int qos;
        } subscribe;
        struct {
        } unsubscribe;
    } arguments;
    delayed_call_t *next;
};

typedef struct {
    MQTTAsync client;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
} cleanup_context_t;

static MQTTAsync client;
static int globally_initialized = 0;
static int initialized = 0;
static spacepi_pubsub_connection_t connection_state;
static connection_callback_list_t *connection_callbacks;
static subscription_callback_trie_t *subscription_callbacks;
static delayed_call_t *delayed_calls;

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
static void safe_mqtt_send(const char *destinationName, int payloadlen, const void *payload, int qos, int retained, MQTTAsync_responseOptions *response, spacepi_token_t *token);
static void safe_mqtt_subscribe(const char *topic, int qos, MQTTAsync_responseOptions *response);
static void safe_mqtt_unsubscribe(const char *topic, MQTTAsync_responseOptions *response);

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
    delayed_calls = NULL;
    return 0;
}

int spacepi_pubsub_cleanup(void) {
    initialized = FALSE;
    int e;
    cleanup_context_t context = {
        .client = client
    };
    if (pthread_cond_init(&context.condition, NULL)) {
        perror("pthread_cond_init");
    }
    if (pthread_mutex_init(&context.mutex, NULL)) {
        perror("pthread_mutex_init");
    }
    MQTTAsync_disconnectOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'D' },
        .struct_version = 1,
        .timeout = MQTT_DISCONNECT_TIMEOUT,
        .onSuccess = destroy_after_disconnect_s,
        .onFailure = destroy_after_disconnect_f,
        .context = &context,
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
    } else if (e != MQTTASYNC_DISCONNECTED) {
        fprintf(stderr, "Failed to disconnect: %s (%d)\n", MQTTAsync_strerror(e), e);
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
    if (e == MQTTASYNC_DISCONNECTED) {
        e = 0;
    } else {
        pthread_mutex_lock(&context.mutex);
        pthread_cond_wait(&context.condition, &context.mutex);
        pthread_mutex_unlock(&context.mutex);
    }
    MQTTAsync_destroy(&client);
    pthread_mutex_destroy(&context.mutex);
    pthread_cond_destroy(&context.condition);
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
    safe_mqtt_send(channel, data_len, data, qos, retain, NULL, NULL);
    return 0;
}

int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token) {
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
    safe_mqtt_send(channel, data_len, data, qos, retain, &opts, token);
    return 0;
}

int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback, void *context) {
    void *data_clone = malloc(data_len);
    if (!data_clone) {
        return ~(errno = ENOBUFS);
    }
    memcpy(data_clone, data, data_len);
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
    safe_mqtt_send(channel, data_len, data, qos, retain, &opts, NULL);
    return 0;
}

int spacepi_wait_token(spacepi_token_t token, unsigned long timeout) {
    if (connection_state != connected) {
        return ~(errno = EINVAL);
    }
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
        safe_mqtt_subscribe(channel, qos, &opts);
        (*trie_it)->qos = qos;
    } else if ((*trie_it)->qos != qos && (*trie_it)->qos != exactly_once) {
        safe_mqtt_subscribe(channel, exactly_once, &opts);
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
        safe_mqtt_unsubscribe(channel, &opts);
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
    return 0;
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
    while (delayed_calls) {
        delayed_call_t *node = delayed_calls;
        delayed_calls = node->next;
        int e;
        const char *type_name;
        switch (node->type) {
            case send:
                e = MQTTAsync_send(client, node->channel, node->arguments.send.payloadlen, node->arguments.send.payload, node->arguments.send.qos, node->arguments.send.retained, node->response_ptr);
                if (node->arguments.send.token) {
                    *node->arguments.send.token = node->response.token;
                }
                free(node->arguments.send.payload);
                type_name = "send";
                break;
            case subscribe:
                e = MQTTAsync_subscribe(client, node->channel, node->arguments.subscribe.qos, node->response_ptr);
                type_name = "subscribe";
                break;
            case unsubscribe:
                e = MQTTAsync_unsubscribe(client, node->channel, node->response_ptr);
                type_name = "unsubscribe";
                break;
            default:
                fprintf(stderr, "Delayed MQTT call does not have a consumer in callback_connected (type=%d)!\n", node->type);
                e = MQTTASYNC_SUCCESS;
                break;
        }
        if (e != MQTTASYNC_SUCCESS) {
            fprintf(stderr, "MQTT %s failed: %s (%d)\n", type_name, MQTTAsync_strerror(e), e);
        }
        free(node->channel);
        free(node);
    }
}

static void destroy_after_disconnect_s(void *context, MQTTAsync_successData *res) {
    cleanup_context_t *c = (cleanup_context_t *) context;
    pthread_cond_signal(&c->condition);
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

static void safe_mqtt_send(const char *destinationName, int payloadlen, const void *payload, int qos, int retained, MQTTAsync_responseOptions *response, spacepi_token_t *token) {
    if (connection_state == connected) {
        int e;
        switch ((e = MQTTAsync_send(client, destinationName, payloadlen, payload, qos, retained, response))) {
            case MQTTASYNC_SUCCESS:
                return;
            case MQTTASYNC_DISCONNECTED:
                break;
            default:
                fprintf(stderr, "MQTT send failed: %s (%d)\n", MQTTAsync_strerror(e), e);
                return;
        }
    }
    delayed_call_t *node = (delayed_call_t *) malloc(sizeof(delayed_call_t));
    if (!node) {
        fputs("MQTT send failed: out of memory.\n", stderr);
        return;
    }
    node->type = send;
    int destinationName_len = strlen(destinationName);
    node->channel = (char *) malloc(destinationName_len);
    if (!node->channel) {
        free(node);
        fputs("MQTT send failed: out of memory.\n", stderr);
        return;
    }
    strcpy(node->channel, destinationName);
    if (response) {
        node->response = *response;
        node->response_ptr = &node->response;
    } else {
        node->response_ptr = NULL;
    }
    node->arguments.send.payloadlen = payloadlen;
    node->arguments.send.payload = malloc(payloadlen);
    if (!node->arguments.send.payload) {
        free(node->channel);
        free(node);
        fputs("MQTT send failed: out of memory.\n", stderr);
        return;
    }
    memcpy(node->arguments.send.payload, payload, payloadlen);
    node->arguments.send.qos = qos;
    node->arguments.send.retained = retained;
    node->arguments.send.token = token;
    if (token) {
        *token = 0;
    }
    node->next = delayed_calls;
    delayed_calls = node;
}

static void safe_mqtt_subscribe(const char *topic, int qos, MQTTAsync_responseOptions *response) {
    if (connection_state == connected) {
        int e;
        switch ((e = MQTTAsync_subscribe(client, topic, qos, response))) {
            case MQTTASYNC_SUCCESS:
                return;
            case MQTTASYNC_DISCONNECTED:
                break;
            default:
                fprintf(stderr, "MQTT subscribe failed: %s (%d)\n", MQTTAsync_strerror(e), e);
                return;
        }
    }
    delayed_call_t *node = (delayed_call_t *) malloc(sizeof(delayed_call_t));
    if (!node) {
        fputs("MQTT subscribe failed: out of memory.\n", stderr);
        return;
    }
    node->type = subscribe;
    int topic_len = strlen(topic);
    node->channel = (char *) malloc(topic_len);
    if (!node->channel) {
        free(node);
        fputs("MQTT subscribe failed: out of memory.\n", stderr);
        return;
    }
    strcpy(node->channel, topic);
    if (response) {
        node->response = *response;
        node->response_ptr = &node->response;
    } else {
        node->response_ptr = NULL;
    }
    node->arguments.subscribe.qos = qos;
    node->next = delayed_calls;
    delayed_calls = node;
}

static void safe_mqtt_unsubscribe(const char *topic, MQTTAsync_responseOptions *response) {
    if (connection_state == connected) {
        int e;
        switch ((e = MQTTAsync_unsubscribe(client, topic, response))) {
            case MQTTASYNC_SUCCESS:
                return;
            case MQTTASYNC_DISCONNECTED:
                break;
            default:
                fprintf(stderr, "MQTT unsubscribe failed: %s (%d)\n", MQTTAsync_strerror(e), e);
                return;
        }
    }
    delayed_call_t *node = (delayed_call_t *) malloc(sizeof(delayed_call_t));
    if (!node) {
        fputs("MQTT unsubscribe failed: out of memory.\n", stderr);
        return;
    }
    node->type = unsubscribe;
    int topic_len = strlen(topic);
    node->channel = (char *) malloc(topic_len);
    if (!node->channel) {
        free(node);
        fputs("MQTT unsubscribe failed: out of memory.\n", stderr);
        return;
    }
    strcpy(node->channel, topic);
    if (response) {
        node->response = *response;
        node->response_ptr = &node->response;
    } else {
        node->response_ptr = NULL;
    }
    node->next = delayed_calls;
    delayed_calls = node;
}
