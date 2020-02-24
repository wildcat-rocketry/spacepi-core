#include <string.h>
#include <pthread.h>
#include <spacepi-private.h>

#ifdef MQTT_HOST
#define MQTT_HOST_STR STR(MQTT_HOST)
#else
#define MQTT_HOST_STR "tcp://localhost:1883"
#endif
#ifdef MQTT_PERSISTENCE_DIR
#define MQTT_PERSISTENCE_DIR /tmp
#endif

pubsub_state_t *pubsub_state = NULL;

int spacepi_pubsub_init(void) {
    static int globally_initialized = 0;
    if (!globally_initialized) {
        MQTTAsync_init_options opts = {
            .struct_id = { 'M', 'Q', 'T', 'G' },
            .struct_version = 0,
            .do_openssl_init = FALSE
        };
        MQTTAsync_global_init(&opts);
    }
    if (pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_ALREADY_INIT);
    }
    CHECK_ERROR(thread_pool_init);
    CHECK_ALLOC(pubsub_state, pubsub_state_t);
    memset(pubsub_state, 0, sizeof(pubsub_state_t));
    CHECK_ERROR_JUMP(free_pubsub_state, pthread_mutex_init, &pubsub_state->mutex, NULL);
    char client_id[23];
    strcpy(client_id, "spacepi");
    CHECK_ERROR_JUMP(destroy_mutex, randomize_string, base_62, 1, 62, client_id + 7, 23 - 7);
    client_id[22] = 0;
    CHECK_ERROR_JUMP_MQTT(destroy_mutex, MQTTAsync_create, &pubsub_state->mqtt, MQTT_HOST_STR, client_id, MQTTCLIENT_PERSISTENCE_DEFAULT, STR(MQTT_PERSISTENCE_DIR));
    CHECK_ERROR_JUMP_MQTT(destroy_mqtt, MQTTAsync_setCallbacks, pubsub_state->mqtt, pubsub_state, NULL, spacepi_private_pubsub_message_arrived, NULL);
    CHECK_ERROR_JUMP(destroy_mqtt, spacepi_private_pubsub_connect, pubsub_state);
    return 0;
    destroy_mqtt:
    MQTTAsync_destroy(pubsub_state->mqtt);
    destroy_mutex:
    pthread_mutex_destroy(&pubsub_state->mutex);
    free_pubsub_state:
    free(pubsub_state);
    RETURN_REPORTED_ERROR();
}
