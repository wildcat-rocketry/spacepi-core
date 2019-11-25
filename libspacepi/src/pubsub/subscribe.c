#include <stdlib.h>
#include <string.h>
#include <spacepi-private.h>

static void message_search(pubsub_subscription_tree_t *tree, char *logical_channel, char *logical_channel_it, const char *channel_it, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
static void message_arrived(pubsub_subscription_tree_t *tree, const char *logical_channel, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
static pubsub_subscription_tree_t *find(pubsub_subscription_tree_t *tree, const char *channel);
static void insert(pubsub_subscription_tree_t **tree, pubsub_subscription_tree_t *node);
static void remove_node(pubsub_subscription_tree_t **tree, pubsub_subscription_tree_t *node);
static void balance(pubsub_subscription_tree_t **tree);
static void stat_tree(pubsub_subscription_tree_t *tree);
static void on_failure(void *context, MQTTAsync_failureData *response);

DEFINE_THREAD_ENQUEUE_6(call_subscription_callback, void *, const char *, const void *, size_t, spacepi_qos_t, int);

int spacepi_subscribe(const char *channel, spacepi_qos_t qos, spacepi_subscription_callback callback, void *context) {
    if (!pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_NOT_INIT);
    }
    CHECK_ALLOC_DEF(node, pubsub_subscription_list_t);
    CHECK_ERROR_JUMP(free_node, pthread_mutex_lock, &pubsub_state->mutex);
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = on_failure,
        .context = NULL,
        .token = 0,
        .onSuccess5 = NULL,
        .onFailure5 = NULL,
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
    pubsub_subscription_tree_t *tree = find(pubsub_state->subscriptions, channel);
    char *channel2;
    if (!tree) {
        CHECK_ALLOC_JUMP(unlock_mutex, tree, pubsub_subscription_tree_t);
        CHECK_ALLOC_ARRAY_JUMP(free_tree, channel2, char, strlen(channel) + 1);
        CHECK_ERROR_JUMP_MQTT(free_channel, MQTTAsync_subscribe, pubsub_state->mqtt, channel, qos, &opts);
        strcpy(channel2, channel);
        tree->channel = channel2;
        tree->qos = qos;
        tree->list = NULL;
        tree->less = NULL;
        tree->greater = NULL;
        insert(&pubsub_state->subscriptions, tree);
        stat_tree(pubsub_state->subscriptions);
        balance(&pubsub_state->subscriptions);
    } else if (tree->qos != qos && tree->qos != exactly_once) {
        CHECK_ERROR_JUMP_MQTT(unlock_mutex, MQTTAsync_subscribe, pubsub_state->mqtt, channel, exactly_once, &opts);
        tree->qos = exactly_once;
    }
    node->callback = callback;
    node->context = context;
    node->next = tree->list;
    tree->list = node;
    CHECK_ERROR_JUMP(unlock_error, pthread_mutex_unlock, &pubsub_state->mutex);
    return 0;

    free_channel:
    free(channel2);
    free_tree:
    free(tree);
    unlock_mutex:
    CHECK_ERROR_JUMP(unlock_error, pthread_mutex_unlock, &pubsub_state->mutex);
    free_node:
    RETURN_REPORTED_ERROR();

    unlock_error:
    CHECK_ERROR_JUMP(free_node, pthread_mutex_init, &pubsub_state->mutex, NULL);
    goto free_node;
}

int spacepi_unsubscribe(const char *channel, spacepi_subscription_callback callback, void *context) {
    if (!pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_NOT_INIT);
    }
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = on_failure,
        .context = NULL,
        .token = 0,
        .onSuccess5 = NULL,
        .onFailure5 = NULL,
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
    CHECK_ERROR(pthread_mutex_lock, &pubsub_state->mutex);
    int err = 1;
    pubsub_subscription_tree_t *tree = find(pubsub_state->subscriptions, channel);
    pubsub_subscription_list_t _list = {
        .callback = NULL,
        .context = NULL,
        .next = tree->list
    };
    for (pubsub_subscription_list_t *it = &_list; it->next;) {
        if ((callback == NULL || it->next->callback == callback) && (context == NULL || it->next->context == context)) {
            pubsub_subscription_list_t *tmp = it->next;
            it->next = tmp->next;
            free(tmp);
        } else {
            it = it->next;
        }
    }
    tree->list = _list.next;
    if (!tree->list) {
        remove_node(&pubsub_state->subscriptions, tree);
        stat_tree(pubsub_state->subscriptions);
        balance(&pubsub_state->subscriptions);
        free(tree);
        CHECK_ERROR_JUMP_MQTT(unlock_mutex, MQTTAsync_unsubscribe, pubsub_state->mqtt, channel, &opts);
    }
    
    err = 0;
    unlock_mutex:
    CHECK_ERROR_JUMP(unlock_error, pthread_mutex_unlock, &pubsub_state->mutex);
    if (err) {
        RETURN_REPORTED_ERROR();
    } else {
        return 0;
    }

    unlock_error:
    CHECK_ERROR(pthread_mutex_init, &pubsub_state->mutex, NULL);
    RETURN_REPORTED_ERROR();
}

int spacepi_private_pubsub_message_arrived(void *context, char *topic_name, int topic_len, MQTTAsync_message *message) {
    pubsub_state_t *state = (pubsub_state_t *) context;
    // Multi-level wildcard matches
    CHECK_ALLOC_ARRAY_DEF_JUMP(end, logical_channel, char, strlen(topic_name) + 1);
    char *logical_it = logical_channel;
    const char *channel_start = topic_name;
    // test/chan/1/bc
    for (const char *channel_it = topic_name; *channel_it; ++channel_it) {
        if (*channel_it == '/') {
            *logical_it = '#';
            strcpy(logical_it + 1, channel_it);
            message_arrived(state->subscriptions, logical_channel, topic_name, message->payload, message->payloadlen, message->qos, message->retained);
            int n = channel_it - channel_start + 1;
            strncpy(logical_it, channel_start, n);
            logical_it += n;
            channel_start += n;
        }
    }
    *logical_it = '#';
    logical_it[1] = 0;
    message_arrived(state->subscriptions, logical_channel, topic_name, message->payload, message->payloadlen, message->qos, message->retained);
    // Single-level wildcard matches (and exact match as a special case)
    message_search(state->subscriptions, logical_channel, logical_channel, topic_name, topic_name, message->payload, message->payloadlen, message->qos, message->retained);

    end:
    return TRUE;
}

static void message_search(pubsub_subscription_tree_t *tree, char *logical_channel, char *logical_channel_it, const char *channel_it, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    const char *next_slash;
    for (next_slash = channel_it; *next_slash && *next_slash != '/'; ++next_slash);
    if (*next_slash) {
        *logical_channel_it = ':';
        logical_channel_it[1] = '/';
        message_search(tree, logical_channel, logical_channel_it + 2, next_slash + 1, channel, data, data_len, qos, retain);
        int n = next_slash - channel_it + 1;
        strncpy(logical_channel_it, channel_it, n);
        message_search(tree, logical_channel, logical_channel_it + n, channel_it + n, channel, data, data_len, qos, retain);
    } else {
        *logical_channel_it = ':';
        logical_channel_it[1] = 0;
        message_arrived(tree, logical_channel, channel, data, data_len, qos, retain);
        strcpy(logical_channel_it, channel_it);
        message_arrived(tree, logical_channel, channel, data, data_len, qos, retain);
    }
}

static void message_arrived(pubsub_subscription_tree_t *tree, const char *logical_channel, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    pubsub_subscription_tree_t *node = find(tree, logical_channel);
    if (node) {
        for (pubsub_subscription_list_t *it = node->list; it; it = it->next) {
            call_subscription_callback(it->callback, it->context, channel, data, data_len, qos, retain);
        }
    }
}

static pubsub_subscription_tree_t *find(pubsub_subscription_tree_t *tree, const char *channel) {
    if (!tree) {
        return NULL;
    }
    int c = strcmp(channel, tree->channel);
    if (c < 0) {
        return find(tree->less, channel);
    } else if (c > 0) {
        return find(tree->greater, channel);
    } else {
        return tree;
    }
}

static void insert(pubsub_subscription_tree_t **tree, pubsub_subscription_tree_t *node) {
    if (!*tree) {
        *tree = node;
        return;
    }
    int c = strcmp(node->channel, (*tree)->channel);
    if (c < 0) {
        insert(&(*tree)->less, node);
    } else if (c > 0) {
        insert(&(*tree)->greater, node);
    } else {
        fflush(stdout);
        fputs("libspacepi: pubsub: subscription insertion failure\n", stderr);
    }
}

static void remove_node(pubsub_subscription_tree_t **tree, pubsub_subscription_tree_t *node) {
    if (!*tree) {
        return;
    }
    int c = strcmp(node->channel, (*tree)->channel);
    if (c < 0) {
        remove_node(&(*tree)->less, node);
    } else if (c > 0) {
        remove_node(&(*tree)->greater, node);
    } else if (node->less) {
        if (node->greater) {
            pubsub_subscription_tree_t **it = &node->greater;
            while ((*it)->less) {
                it = &(*it)->less;
            }
            /*
                   8
               4       C
             2   6   A   F
            1 3 5 7 9 B D
                         E
            tree = C
            node = C
            node->less = A
            node->greater = F
            it = D
            */
            (*it)->less = node->less;
            /*
               D
             A   E
            9 B
            */
            *tree = *it;
            /*
                   8
               4       D
             2   6   A   E
            1 3 5 7 9 B

               C
             A   F
            9 B
            */
            it = &(*it)->greater;
            while (*it) {
                it = &(*it)->greater;
            }
            *it = node->greater;
            /*
                   8
               4       D
             2   6   A   E
            1 3 5 7 9 B   F
            */
        } else {
            *tree = node->less;
        }
    } else if (node->greater) {
        *tree = node->greater;
    } else {
        *tree = NULL;
    }
}

static void balance(pubsub_subscription_tree_t **tree) {
    if (!*tree) {
        return;
    }
    if ((*tree)->children < (1 << ((*tree)->depth - 1))) {
        int gc, lc, glc, lgc, gd, ld, gld, lgd;
        if ((*tree)->greater) {
            gc = (*tree)->greater->children;
            gd = (*tree)->greater->depth;
            if ((*tree)->greater->less) {
                glc = (*tree)->greater->less->children;
                gld = (*tree)->greater->less->depth;
            } else {
                glc = 0;
                gld = 0;
            }
        } else {
            gc = 0;
            gd = 0;
            glc = 0;
            gld = 0;
        }
        if ((*tree)->less) {
            lc = (*tree)->greater->children;
            ld = (*tree)->greater->depth;
            if ((*tree)->less->greater) {
                lgc = (*tree)->less->greater->children;
                lgd = (*tree)->less->greater->depth;
            } else {
                lgc = 0;
                lgd = 0;
            }
        } else {
            lc = 0;
            ld = 0;
            lgc = 0;
            lgd = 0;
        }
        if (gc < (1 << (gd - 1))) {
            if (glc < (1 << (gld - 1))) {
                // TODO less double rotate
            } else {
                // TODO less rotate
            }
        } else if (lc < (1 << (ld - 1))) {
            if (lgc < (1 << (lgd - 1))) {
                // TODO greater double rotate
            } else {
                // TODO greater rotate
            }
        }
    }
}

static void stat_tree(pubsub_subscription_tree_t *tree) {
    if (!tree) {
        return;
    }
    stat_tree(tree->less);
    stat_tree(tree->greater);
    if (tree->less) {
        if (tree->greater) {
            if (tree->less->depth < tree->greater->depth) {
                tree->depth = tree->greater->depth + 1;
            } else {
                tree->depth = tree->less->depth + 1;
            }
            tree->children = tree->less->children + tree->greater->children + 1;
        } else {
            tree->depth = tree->less->depth + 1;
            tree->children = tree->less->children + 1;
        }
    } else if (tree->greater) {
        tree->depth = tree->greater->depth + 1;
        tree->children = tree->greater->children + 1;
    } else {
        tree->depth = 1;
        tree->children = 1;
    }
}

static void on_failure(void *context, MQTTAsync_failureData *response) {
    fflush(stdout);
    if (response->message) {
        fprintf(stderr, "Failed to connect to server: %s (%d)\n", response->message, response->code);
    } else {
        fprintf(stderr, "Failed to connect to server: %d\n", response->code);
    }
}
