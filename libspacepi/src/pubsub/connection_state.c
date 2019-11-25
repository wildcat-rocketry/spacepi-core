#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <spacepi-private.h>

DEFINE_THREAD_ENQUEUE_2(call_connection_handler, void *, spacepi_pubsub_connection_t);
DEFINE_THREAD_ENQUEUE_1(call_server_down_handler, void *);

spacepi_pubsub_connection_t spacepi_pubsub_connected(void) {
    if (!pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_NOT_INIT);
    }
    return pubsub_state->conn;
}

int spacepi_pubsub_connection_handler(spacepi_connection_callback callback, void *context) {
    if (!pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_NOT_INIT);
    }
    CHECK_ALLOC_DEF(node, pubsub_connection_handler_list_t);
    node->callback = callback;
    node->context = context;
    CHECK_ERROR_JUMP(free_node, pthread_mutex_lock, &pubsub_state->mutex);
    node->next = pubsub_state->connection_handlers;
    pubsub_state->connection_handlers = node;
    CHECK_ERROR_JUMP(unlock_error, pthread_mutex_unlock, &pubsub_state->mutex);
    return 0;

    unlock_error:
    CHECK_ERROR(pthread_mutex_init, &pubsub_state->mutex, NULL);
    RETURN_REPORTED_ERROR();

    free_node:
    free(node);
    RETURN_REPORTED_ERROR();
}

void spacepi_private_pubsub_connection_lost(void *context, char *cause) {
    pubsub_state_t *ctx = (pubsub_state_t *) context;
    fflush(stdout);
    if (cause) {
        fprintf(stderr, "Connection lost: %s\n", cause);
    } else {
        fputs("Connection lost.\n", stderr);
    }
    if (ctx->conn != disconnected) {
        ctx->conn = disconnected;
        for (pubsub_connection_handler_list_t *it = ctx->connection_handlers; it; it = it->next) {
            CHECK_ERROR_JUMP(after, call_connection_handler, it->callback, it->context, ctx->conn);
            after:
            1;
        }
        CHECK_ERROR_JUMP(conn_err, spacepi_private_pubsub_connect, ctx, FALSE);
        return;
    }

    conn_err:
    fflush(stdout);
    fputs("Unable to reestablish connection.\n", stderr);
    if (pubsub_state->server_down_callback) {
        CHECK_ERROR_JUMP(do_exit, call_server_down_handler, pubsub_state->server_down_callback, pubsub_state->server_down_context);
    } else {
        do_exit:
        exit(EXIT_FAILURE);
    }
}

int spacepi_handle_server_down(spacepi_server_down_callback callback, void *context) {
    if (!pubsub_state) {
        RETURN_ERROR_SPACEPI(LIB_NOT_INIT);
    }
    pubsub_state->server_down_callback = callback;
    pubsub_state->server_down_context = context;
    return 0;
}
