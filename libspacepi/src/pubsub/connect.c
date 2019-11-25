#include <stdio.h>
#include <pthread.h>
#include <spacepi-private.h>

#ifndef MQTT_KEEP_ALIVE
#define MQTT_KEEP_ALIVE 2
#endif
#ifndef MQTT_CONNECT_TIMEOUT
#define MQTT_CONNECT_TIMEOUT 5
#endif
#ifndef MQTT_RETRY_INTERVAL_MIN
#define MQTT_RETRY_INTERVAL_MIN 1
#endif
#ifndef MQTT_RETRY_INTERVAL_MAX
#define MQTT_RETRY_INTERVAL_MAX 64
#endif

typedef struct {
    pubsub_state_t *state;
    pthread_cond_t lock;
} connect_context_t;

static void on_success(void *context, MQTTAsync_successData *response);
static void on_failure(void *context, MQTTAsync_failureData *response);

int spacepi_private_pubsub_connect(pubsub_state_t *state, int clean) {
    if (state->conn == connected) {
        RETURN_ERROR_SPACEPI(ALREADY_CONNECTED);
    }
    CHECK_ERROR(pthread_mutex_lock, &state->mutex);
    int err = 1;
    CHECK_ALLOC_DEF_JUMP(free_cond, ctx, connect_context_t);
    ctx->state = state;
    CHECK_ERROR_JUMP(free_context, pthread_cond_init, &ctx->lock, NULL);
    MQTTAsync_connectOptions opts = {
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
        .onSuccess = on_success,
        .onFailure = on_failure,
        .context = ctx,
        .serverURIcount = 0,
        .serverURIs = NULL,
        .MQTTVersion = MQTTVERSION_DEFAULT,
        .automaticReconnect = FALSE,
        .minRetryInterval = MQTT_RETRY_INTERVAL_MIN,
        .maxRetryInterval = MQTT_RETRY_INTERVAL_MAX,
        .binarypwd = {
            .len = 0,
            .data = NULL
        },
        .cleanstart = FALSE,
        .connectProperties = NULL,
        .willProperties = NULL,
        .onSuccess5 = NULL,
        .onFailure5 = NULL
    };
    state->conn = connecting;
    CHECK_ERROR_JUMP_MQTT(free_cond, MQTTAsync_connect, state->mqtt, &opts);
    if (state->conn == connecting) {
        pthread_cond_wait(&ctx->lock, &state->mutex);
    }
    pthread_mutex_unlock(&state->mutex);
    if (state->conn == connected) {
        err = 0;
    }
    free_cond:
    pthread_cond_destroy(&ctx->lock);
    free_context:
    free(ctx);
    unlock_mutex:
    pthread_mutex_unlock(&state->mutex);
    if (err) {
        RETURN_REPORTED_ERROR();
    } else {
        return 0;
    }
}

static void on_success(void *context, MQTTAsync_successData *response) {
    connect_context_t *ctx = (connect_context_t *) context;
    ctx->state->conn = connected;
    puts("Connected to server.");
    pthread_cond_signal(&ctx->lock);
}

static void on_failure(void *context, MQTTAsync_failureData *response) {
    connect_context_t *ctx = (connect_context_t *) context;
    fflush(stdout);
    if (response->message) {
        fprintf(stderr, "Failed to connect to server: %s (%d)\n", response->message, response->code);
    } else {
        fprintf(stderr, "Failed to connect to server: %d\n", response->code);
    }
    pthread_cond_signal(&ctx->lock);
}
