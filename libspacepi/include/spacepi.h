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
} spacepi_pubsub_connection_t;

typedef int spacepi_token_t;

/*
 * spacepi_connection_callback is called after the status of the connection to the local MQTT server changes
 * 
 * Parameters:
 *  - context: The context object passed into spacepi_pubsub_connection_handler()
 *  - connection: The current state of the connection
 */
typedef void (*spacepi_connection_callback)(void *context, spacepi_pubsub_connection_t connection);
/*
 * spacepi_published_callback is called after a message has been published from this process onto the local MQTT server
 * 
 * Parameters:
 *  - context: The context object passed into spacepi_publish_callback()
 *  - channel: The channel the message was published to
 *  - data: The data that was sent in the message
 *  - data_len: The length of the data
 *  - qos: The QOS setting for the message
 *  - retain: If the message was saved to the channel
 */
typedef void (*spacepi_published_callback)(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
/*
 * spacepi_subscription_callback is called after a message has been received to this process from the local MQTT server
 * 
 * Parameters:
 *  - context: The context object passed into spacepi_subscribe()
 *  - channel: The channel the message was received from
 *  - data: The data that was received in the message
 *  - data_len: The length of the data
 *  - qos: The QOS setting for the message
 *  - retain: If the message was saved to the channel
 */
typedef void (*spacepi_subscription_callback)(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);

/*
 * spacepi_pubsub_init initializes the publish/subscribe framework for communicating between processes and hosts
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_pubsub_init(void);
/*
 * spacepi_pubsub_cleanup destroys all state data, frees all memory, and disconnects from the local MQTT server
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_pubsub_cleanup(void);
/*
 * spacepi_pubsub_connected determines the current state of the connection to the local MQTT server
 * 
 * Return: The current state of the connection to the server
 */
spacepi_pubsub_connection_t spacepi_pubsub_connected(void);
/*
 * spacepi_pubsub_connection_handler registers a callback to be called every time the state of the connection to the local MQTT server changes
 * 
 * Parameters:
 *  - callback: The function to call when the state changes
 *  - context: A context object to pass to the callback function
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_pubsub_connection_handler(spacepi_connection_callback callback, void *context);
/*
 * spacepi_publish sends a message to other processes which have subscribed to the channel
 * 
 * Parameters:
 *  - channel: The channel to publish the message to.  Valid characters are a-z, 0-9, '/', and '-'.
 *  - data: The data to send along with the message
 *  - data_len: The length of the data
 *  - qos: The QOS setting to use
 *  - retain: Whether or not the message should be stored in the channel and immediately sent to any processes which subscibe to the channel
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_publish(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
/*
 * spacepi_publish_token sends a message to other processes which have subscribed to the channel and saves a token that
 * can later be used to wait until the message has been fully sent to the server.
 * 
 * Parameters:
 *  - channel: The channel to publish the message to.  Valid characters are a-z, 0-9, '/', and '-'.
 *  - data: The data to send along with the message
 *  - data_len: The length of the data
 *  - qos: The QOS setting to use
 *  - retain: Whether or not the message should be stored in the channel and immediately sent to any processes which subscibe to the channel
 *  - token: (output) The token that can be used with spacepi_wait_token() to wait until the message has been published
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_publish_token(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_token_t *token);
/*
 * spacepi_publish_callback sends a message to other processes which have subscribed to the channel and calls a
 * function after the message has been fully sent to the server.
 * 
 * Parameters:
 *  - channel: The channel to publish the message to.  Valid characters are a-z, 0-9, '/', and '-'.
 *  - data: The data to send along with the message
 *  - data_len: The length of the data
 *  - qos: The QOS setting to use
 *  - retain: Whether or not the message should be stored in the channel and immediately sent to any processes which subscibe to the channel
 *  - callback: The function to call after the message has been fully sent to the server
 *  - context: A context object to pass to the callback
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_publish_callback(const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, spacepi_published_callback callback, void *context);
/*
 * spacepi_wait_token blocks until the event specified by a token completes.  Currently the only event this can be is a
 * message being published to the server (see spacepi_publish_token).
 * 
 * Parameters:
 *  - token: The token from the process to wait for
 *  - timeout: The maximum amount of time in milliseconds to block for
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_wait_token(spacepi_token_t token, unsigned long timeout);
/*
 * spacepi_subscribe subscribes to a channel so that all messages published to that channel are passed to a callback
 * function in this program
 * 
 * Parameters:
 *  - channel: The channel to subscribe to.  Valid characters are a-z, 0-9, '/', and '-'.  In addition, '+' may be used
 *             as a wildcard to subscribe to all channels in which the '+' can be replaced by any string not containing
 *             a '/', and the '#' may be used as a wildcard to match any characters.  The '+' must be surrounded by
 *             either '/' or nothing on both sides, and no characters may follow the '#'.
 *  - qos: The minimum QOS to receive from this subscription
 *  - callback: The function to call every time a message is published to this channel from another process
 *  - context: A context object to pass to the callback
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_subscribe(const char *channel, spacepi_qos_t qos, spacepi_subscription_callback callback, void *context);
/*
 * spacepi_unsubscribe unsubscribes from a channel so no more messages are received
 * 
 * Parameters:
 *  - channel: The channel to unsubscribe from.  This must be the exact same string (value) as passed into spacepi_subscribe().
 *  - callback: If specified, only subscriptions with this function will be unregistered (otherwise pass NULL)
 *  - context: If specified, only subscriptions with this context object will be unregistered (otherwise pass NULL)
 * 
 * Return: 0 on success or a negative number with errno set on error
 * Even if an error state occurs no more messages will be received by any subscriptions matching this channel, callback, and context.
 */
int spacepi_unsubscribe(const char *channel, spacepi_subscription_callback callback, void *context);

/* Random functions */

/*
 * base_62 contains all of the characters valid in base 62 in order (along with a terminating null character)
 */
extern const char const base_62[63];

/*
 * random_reseed reseeds the random number generator with the current system time
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int random_reseed(void);
/*
 * randomize_string generates a random string with a given alphabet
 * 
 * Parameters:
 *  - alphabet: An array of all possible values of a single word in the string
 *  - word_size: The number of bytes which make up each word in the string
 *  - alphabet_size: The number of words inside the alphabet array
 *  - buffer: The buffer to store the generated random string into
 *  - num_words: The number of words to store into the buffer
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int randomize_string(const void *alphabet, size_t word_size, size_t alphabet_size, void *buffer, size_t num_words);

#endif
