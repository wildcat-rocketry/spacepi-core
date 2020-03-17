#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <spacepi-private.h>

typedef struct {
    pubsub_state_t *pubsub;
    spacepi_published_callback callback;
    void *context;
    const char *channel;
    const void *data;
    size_t data_len;
    spacepi_qos_t qos;
    int retain;
} publish_context_t;

static void on_success(void *context, MQTTAsync_successData *response);
static void on_failure(void *context, MQTTAsync_failureData *response);

DEFINE_THREAD_ENQUEUE_7(call_published_callback, void *, const char *, const void *, size_t, spacepi_qos_t, int, int);

#define PUBLISH_HEAD() do { \
        if (!pubsub_state) { \
            RETURN_ERROR_SPACEPI(LIB_NOT_INIT); \
        } \
        CHECK_ERROR(pthread_mutex_lock, &pubsub_state->mutex); \
        CHECK_ERROR_JUMP(unlock_error, pthread_mutex_unlock, &pubsub_state->mutex); \
    } while (0)

#define PUBLISH_TAIL() do { \
        return 0; \
        unlock_error: \
        CHECK_ERROR(pthread_mutex_init, &pubsub_state->mutex, NULL); \
        RETURN_REPORTED_ERROR(); \
    } while (0)

int spacepi_publish(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    PUBLISH_HEAD();
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = NULL,
        .context = pubsub_state,
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
    CHECK_ERROR_MQTT(spacepi_pubsub_send_filtered, pubsub_state->mqtt, channel, data_len, data, qos, retain, &opts);
    return 0;
    RETURN_REPORTED_ERROR();
    PUBLISH_TAIL();
}

int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token) {
    PUBLISH_HEAD();
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = NULL,
        .onFailure = NULL,
        .context = pubsub_state,
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
    CHECK_ERROR_MQTT(spacepi_pubsub_send_filtered, pubsub_state->mqtt, channel, data_len, data, qos, retain, &opts);
    *token = (spacepi_token_t) opts.token;
    return 0;
    RETURN_REPORTED_ERROR();
    PUBLISH_TAIL();
}

int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback, void *context) {
    PUBLISH_HEAD();
    CHECK_ALLOC_DEF(state, publish_context_t);
    state->pubsub = pubsub_state;
    state->callback = callback;
    state->context = context;
    CHECK_ALLOC_ARRAY_DEF_JUMP(free_state, channel2, char, strlen(channel) + 1);
    strcpy(channel2, channel);
    state->channel = channel2;
    void *data2;
    CHECK_ALLOC_ARRAY_JUMP(free_channel, data2, char, data_len);
    memcpy(data2, data, data_len);
    state->data = data2;
    state->data_len = data_len;
    state->qos = qos;
    state->retain = retain;
    MQTTAsync_responseOptions opts = {
        .struct_id = { 'M', 'Q', 'T', 'R' },
        .struct_version = 1,
        .onSuccess = on_success,
        .onFailure = on_failure,
        .context = pubsub_state,
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
    CHECK_ERROR_JUMP_MQTT(free_data, spacepi_pubsub_send_filtered, pubsub_state->mqtt, channel, data_len, data, qos, retain, &opts);
    return 0;
    free_data:
    free(data2);
    free_channel:
    free(channel2);
    free_state:
    free(state);
    RETURN_REPORTED_ERROR();
    PUBLISH_TAIL();
}

int spacepi_wait_token(spacepi_token_t token, unsigned long timeout) {
    PUBLISH_HEAD();
    CHECK_ERROR_MQTT(MQTTAsync_waitForCompletion, pubsub_state->mqtt, token, timeout);
    PUBLISH_TAIL();
}

static void on_success(void *context, MQTTAsync_successData *response) {
    publish_context_t *ctx = (publish_context_t *) context;
    call_published_callback(ctx->callback, ctx->context, ctx->channel, ctx->data, ctx->data_len, ctx->qos, ctx->retain, FALSE);
    free((char *) ctx->channel);
    free((void *) ctx->data);
    free(ctx);
}

static void on_failure(void *context, MQTTAsync_failureData *response) {
    publish_context_t *ctx = (publish_context_t *) context;
    fflush(stdout);
    if (response->message) {
        fprintf(stderr, "Failed to send message: %s (%d)\n", response->message, response->code);
    } else {
        fprintf(stderr, "Failed to send message: %d\n", response->code);
    }
    call_published_callback(ctx->callback, ctx->context, ctx->channel, ctx->data, ctx->data_len, ctx->qos, ctx->retain, TRUE);
    free((char *) ctx->channel);
    free((void *) ctx->data);
    free(ctx);
}
