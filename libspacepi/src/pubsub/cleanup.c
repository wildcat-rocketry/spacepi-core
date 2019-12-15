#include <stdlib.h>
#include <pthread.h>
#include <spacepi-private.h>

#ifndef MQTT_DISCONNECT_TIMEOUT
#define MQTT_DISCONNECT_TIMEOUT 2000
#endif

typedef struct {
    pubsub_state_t *state;
    pthread_cond_t lock;
} disconnect_context_t;

static void on_success(void *context, MQTTAsync_successData *response);
static void on_failure(void *context, MQTTAsync_failureData *response);

int spacepi_pubsub_cleanup(void) {
    if (!pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_NOT_INIT);
    }
    int err = 0;
    if (pubsub_state->conn == sc_connected) {
        err = 1;
        disconnect_context_t ctx;
        ctx.state = pubsub_state;
        CHECK_ERROR_JUMP(end_if, pthread_cond_init, &ctx.lock, NULL);
        MQTTAsync_disconnectOptions opts = {
            .struct_id = { 'M', 'Q', 'T', 'D' },
            .struct_version = 1,
            .timeout = MQTT_DISCONNECT_TIMEOUT,
            .onSuccess = on_success,
            .onFailure = on_failure,
            .context = &ctx,
            .properties = {
                .count = 0,
                .max_count = 0,
                .length = 0,
                .array = NULL
            },
            .reasonCode = MQTTREASONCODE_NORMAL_DISCONNECTION,
            .onSuccess5 = NULL,
            .onFailure5 = NULL
        };
        pthread_mutex_lock(&pubsub_state->mutex);
        CHECK_ERROR_JUMP_MQTT(free_cond, MQTTAsync_disconnect, pubsub_state->mqtt, &opts);
        if (pubsub_state->conn != sc_disconnected) {
            pthread_cond_wait(&ctx.lock, &pubsub_state->mutex);
        }
        pthread_mutex_unlock(&pubsub_state->mutex);
        err = 0;
        free_cond:
        pthread_cond_destroy(&ctx.lock);
    }
    end_if:
    pthread_mutex_destroy(&pubsub_state->mutex);
    MQTTAsync_destroy(&pubsub_state->mqtt);
    free(pubsub_state);
    pubsub_state = NULL;
    if (err) {
        RETURN_REPORTED_ERROR();
    } else {
        return 0;
    }
}

static void on_success(void *context, MQTTAsync_successData *response) {
    disconnect_context_t *ctx = (disconnect_context_t *) context;
    ctx->state->conn = sc_disconnected;
    pthread_cond_signal(&ctx->lock);
}

static void on_failure(void *context, MQTTAsync_failureData *response) {
    disconnect_context_t *ctx = (disconnect_context_t *) context;
    ctx->state->conn = sc_disconnected;
    pthread_cond_signal(&ctx->lock);
}
