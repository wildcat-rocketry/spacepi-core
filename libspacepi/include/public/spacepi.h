#ifndef SPACEPI_H
#define SPACEPI_H

/* Standard library includes */

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
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

/* Error handling framework */

typedef union {
    int error_code;
    struct {
#if INT_MAX == 127
        unsigned code:6;
#elif INT_MAX == 32767
        unsigned code:14;
#elif INT_MAX == 2147483647
        unsigned code:30;
#elif INT_MAX == 9223372036854775807
        unsigned code:62;
#else
#error Invalid int size.
#endif
        enum {
            system_err = 0,
            spacepi = 1,
            mqtt = 2
        } type:2;
    } data;
} spacepi_error_t;

typedef enum {
    LIB_ALREADY_INIT = 0,
    SPACEPI_ERROR_CASCADE,
    ALREADY_CONNECTED,
    LIB_NOT_INIT,
    UNKNOWN_ERROR
} spacepi_errors_t;

const char *spacepi_strerror(spacepi_error_t spacepi_errno);
void spacepi_perror(const char *func, const char *file, int line);
void spacepi_trace(const char *file, int line);

#define TRACE() do { spacepi_trace(__FILE__, __LINE__); } while (0)
#define SET_ERROR(type, val) do { errno = ((spacepi_error_t) { .data = { CONCAT(.ty, pe) = type, .code = val }}).error_code; } while (0)
#define SET_ERROR_SYSTEM(val) SET_ERROR(system_err, val)
#define RETURN_ERROR(type, val) do { SET_ERROR(type, val); return -1; } while (0)
#define RETURN_ERROR_SYSTEM(val) RETURN_ERROR(system_err, val)
#define RETURN_REPORTED_ERROR() do { SET_ERROR(spacepi, SPACEPI_ERROR_CASCADE); return -1; } while (0)
#define CHECK_ERROR(func, ...) do { if (func(__VA_ARGS__) < 0) { spacepi_perror(STR(func), __FILE__, __LINE__); RETURN_REPORTED_ERROR(); } } while (0)
#define CHECK_ALLOC(var, type) do { var = (type *) malloc(sizeof(type)); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); RETURN_ERROR_SYSTEM(ENOBUFS); } } while (0)
#define CHECK_ALLOC_DEF(var, type) type *var; CHECK_ALLOC(var, type)
#define CHECK_ALLOC_ARRAY(var, type, count) do { var = (type *) malloc(sizeof(type) * (count)); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); RETURN_ERROR_SYSTEM(ENOBUFS); } } while (0)
#define CHECK_ALLOC_ARRAY_DEF(var, type, count) var *type; CHECK_ALLOC_ARRAY(var, type, count)
#define JUMP_ERROR(label, type, val) do { SET_ERROR(type, val); goto label; } while (0)
#define JUMP_ERROR_SYSTEM(label, val) JUMP_ERROR(label, system_err, val)
#define CHECK_ERROR_JUMP(label, func, ...) do { if (func(__VA_ARGS__) < 0) { spacepi_perror(STR(func), __FILE__, __LINE__); goto label; } } while (0)
#define CHECK_ALLOC_JUMP(label, var, type) do { var = (type *) malloc(sizeof(type)); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); JUMP_ERROR_SYSTEM(label, ENOBUFS); } } while (0)
#define CHECK_ALLOC_DEF_JUMP(label, var, type) type *var; CHECK_ALLOC_JUMP(label, var, type)
#define CHECK_ALLOC_ARRAY_JUMP(label, var, type, count) do { var = (type *) malloc(sizeof(type) * count); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); JUMP_ERROR_SYSTEM(label, ENOBUFS); } } while (0)
#define CHECK_ALLOC_ARRAY_DEF_JUMP(label, var, type, count) type *var; CHECK_ALLOC_ARRAY_JUMP(label, var, type, count)

/* Pub/sub framework */

typedef enum {
    at_most_once = 0,
    at_least_once = 1,
    exactly_once = 2
} spacepi_qos_t;

typedef enum {
    disconnected = 0,
    connected = 1,
    connecting = 2
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
 *  - error: If an error occurred while publishing
 */
typedef void (*spacepi_published_callback)(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain, int error);
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
 * spacepi_server_down_callback is called after being disconnected and unable to reconnect to the server.  If not set,
 * the program will exit when unable to reconnect.
 * 
 * Parameters:
 *  - context: The context object passed into spacepi_handle_server_down()
 */
typedef void (*spacepi_server_down_callback)(void *context);

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
 * spacepi_handle_server_down sets a callabck to be called after being disconnected from the server and unable to
 * reconnect.  If not set, the program will exit when unable to reconnect.  Unlike the other callback functions, only
 * one function may be called for this, so a call to this function resets the behavior.  If the callback is NULL, the
 * default behavior of exiting the program will be restored.
 * 
 * Parameters:
 *  - callback: The function to call when unable to reconnect
 *  - context: A context object to pass to the callback function
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int spacepi_handle_server_down(spacepi_server_down_callback callback, void *context);
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

/* Thread pooling framework */

/*
 * thread_pool_init initializes the thread pool, or does nothing if the thread pool has already been initialized.
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int thread_pool_init(void);

/*
 * thread_enqueue enqueues a new task to run on the thread pool.  This function should not be called directly.  Instead,
 * use DEFINE_THREAD_ENQUEUE_n().
 * 
 * Parameters:
 *  - A trampoline function that calls the callback
 *  - The context to give to the trampoline function to call the callback properly.
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int thread_enqueue(void (*trampoline)(void *context), void *context);

/*
 * DEFINE_THREAD_ENQUEUE_n defines a new function that enqueues a different function that takes n arguments to the
 * thread pool.  The first argument to this macro is the name of the function to write.  The following arguments are the
 * types of the arguments to the function to add the the threading queue.
 * 
 * Example:
 * 
 * #include <stdio.h>
 * #include <spacepi.h>
 * 
 * void my_callback(int a, int b);
 * 
 * DEFINE_THREAD_ENQUEUE_2(add_func, int, int);
 * 
 * int main() {
 *     // Using the error framework
 *     CHECK_ERROR(thread_pool_init);
 *     CHECK_ERROR(add_func, my_callback, 2, 4);
 * 
 *     // Not using the error framework
 *     thread_pool_init();
 *     add_func(my_callback, 2, 4);
 * 
 *     return 0;
 * }
 * 
 * void my_callback(int a, int b) {
 *     printf("a = %d, b = %d\n", a, b);
 * }
 * 
 * // Prints out "a = 2, b = 4"
 * 
 * Macro Parameters:
 *  - The name of the function to write
 *  - The argument types for the function
 * 
 * Function Parameters:
 *  - The callback function
 *  - The parameters to the callback function
 * 
 * Return: 0 on success or a negative number with errno set on error
 */

#define DEFINE_THREAD_ENQUEUE_0(func_name) \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        ((void (*)()) context)(); \
    } \
    static int func_name(void (*func)()) { \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), func); \
    }

#define DEFINE_THREAD_ENQUEUE_1(func_name, arg1) \
    typedef struct { \
        void (*func)(arg1); \
        arg1 a1; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1), arg1 a1) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_2(func_name, arg1, arg2) \
    typedef struct { \
        void (*func)(arg1, arg2); \
        arg1 a1; \
        arg2 a2; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2), arg1 a1, arg2 a2) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_3(func_name, arg1, arg2, arg3) \
    typedef struct { \
        void (*func)(arg1, arg2, arg3); \
        arg1 a1; \
        arg2 a2; \
        arg3 a3; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2, ctx->a3); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2, arg3), arg1 a1, arg2 a2, arg3 a3) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        ctx->a3 = a3; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_4(func_name, arg1, arg2, arg3, arg4) \
    typedef struct { \
        void (*func)(arg1, arg2, arg3, arg4); \
        arg1 a1; \
        arg2 a2; \
        arg3 a3; \
        arg4 a4; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2, ctx->a3, ctx->a4); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2, arg3, arg4), arg1 a1, arg2 a2, arg3 a3, arg4 a4) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        ctx->a3 = a3; \
        ctx->a4 = a4; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_5(func_name, arg1, arg2, arg3, arg4, arg5) \
    typedef struct { \
        void (*func)(arg1, arg2, arg3, arg4, arg5); \
        arg1 a1; \
        arg2 a2; \
        arg3 a3; \
        arg4 a4; \
        arg5 a5; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2, ctx->a3, ctx->a4, ctx->a5); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2, arg3, arg4, arg5), arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        ctx->a3 = a3; \
        ctx->a4 = a4; \
        ctx->a5 = a5; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_6(func_name, arg1, arg2, arg3, arg4, arg5, arg6) \
    typedef struct { \
        void (*func)(arg1, arg2, arg3, arg4, arg5, arg6); \
        arg1 a1; \
        arg2 a2; \
        arg3 a3; \
        arg4 a4; \
        arg5 a5; \
        arg6 a6; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2, ctx->a3, ctx->a4, ctx->a5, ctx->a6); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2, arg3, arg4, arg5, arg6), arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        ctx->a3 = a3; \
        ctx->a4 = a4; \
        ctx->a5 = a5; \
        ctx->a6 = a6; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_7(func_name, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    typedef struct { \
        void (*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7); \
        arg1 a1; \
        arg2 a2; \
        arg3 a3; \
        arg4 a4; \
        arg5 a5; \
        arg6 a6; \
        arg7 a7; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2, ctx->a3, ctx->a4, ctx->a5, ctx->a6, ctx->a7); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7), arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        ctx->a3 = a3; \
        ctx->a4 = a4; \
        ctx->a5 = a5; \
        ctx->a6 = a6; \
        ctx->a7 = a7; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#define DEFINE_THREAD_ENQUEUE_8(func_name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    typedef struct { \
        void (*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); \
        arg1 a1; \
        arg2 a2; \
        arg3 a3; \
        arg4 a4; \
        arg5 a5; \
        arg6 a6; \
        arg7 a7; \
        arg8 a8; \
    } CONCAT(func_name, __spacepi_trampoline_ctx); \
    static void CONCAT(func_name, __spacepi_trampoline)(void *context) { \
        CONCAT(func_name, __spacepi_trampoline_ctx) *ctx = (CONCAT(func_name, __spacepi_trampoline_ctx) *) context; \
        ctx->func(ctx->a1, ctx->a2, ctx->a3, ctx->a4, ctx->a5, ctx->a6, ctx->a7, ctx->a8); \
        free(ctx); \
    } \
    static int func_name(void (*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8), arg1 a1, arg2 a2, arg3 a3, arg4 a4, arg5 a5, arg6 a6, arg7 a7, arg8 a8) { \
        CHECK_ALLOC_DEF(ctx, CONCAT(func_name, __spacepi_trampoline_ctx)); \
        ctx->func = func; \
        ctx->a1 = a1; \
        ctx->a2 = a2; \
        ctx->a3 = a3; \
        ctx->a4 = a4; \
        ctx->a5 = a5; \
        ctx->a6 = a6; \
        ctx->a7 = a7; \
        ctx->a8 = a8; \
        return thread_enqueue(CONCAT(func_name, __spacepi_trampoline), ctx); \
    }

#endif