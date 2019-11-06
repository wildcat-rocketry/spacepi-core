#ifndef SPACEPI_H
#define SPACEPI_H

/* Standard library includes */

#include <stddef.h>
#include <time.h>

/* Booleans */

#ifdef TRUE
#if TRUE != 1
#error TRUE != 1
#endif
#else
#define TRUE 1
#endif
#ifdef FALSE
#if FALSE != 0
#error FALSE != 0
#endif
#else
#define FALSE 0
#endif

/* Useful macros */

#ifndef STR
#define _STR(x) # x
#define STR(x) _STR(x)
#endif

#ifndef CONCAT
#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)
#endif

/* Pub/sub framework */

typedef enum {
    at_most_once = 0,
    at_least_once = 1,
    exactly_once = 2
} spacepi_qos_t;

typedef enum {
    disconnected = 0,
    connected = 1
} spacepi_pubsub_connection;

typedef int spacepi_token_t;

typedef void (*spacepi_connection_callback)(void *context, spacepi_pubsub_connection connection);
typedef void (*spacepi_published_callback)(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
typedef void (*spacepi_subscription_callback)(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);

int spacepi_pubsub_init(void);
int spacepi_pubsub_cleanup(void);
spacepi_pubsub_connection spacepi_pubsub_connected(void);
int spacepi_pubsub_connection_handler(spacepi_connection_callback callback, void *context);
int spacepi_publish(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token);
int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback, void *context);
int spacepi_wait_token(spacepi_token_t token, unsigned long timeout);
int spacepi_subscribe(const char *channel, spacepi_qos_t qos, spacepi_subscription_callback callback, void *context);
int spacepi_unsubscribe(const char *channel, spacepi_subscription_callback callback, void *context);

/* Random functions */

extern const char const base_62[63];

int random_reseed(void);
int randomize_string(const void *alphabet, size_t word_size, size_t alphabet_size, void *buffer, size_t num_words);

#endif
