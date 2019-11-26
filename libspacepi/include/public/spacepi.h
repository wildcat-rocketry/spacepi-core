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

/*
 * STR converts literal text to a string.  For example, STR(abc) is replace with "abc" during preprocessing.  Note: if
 * you use this with a variable name, it will create a string that contains the name of the variable.
 * 
 * Parameters: Literal text
 * 
 * Returns: The text as a C string
 */
#ifndef STR
#define _STR(x) # x
#define STR(x) _STR(x)
#endif

/*
 * CONCAT concatenates two sections of literal text and forms a new literal (can be used for identifiers).  Note: if you
 * use this with two variable names, it will concatenate the variable names instead of the values of the variables.
 * 
 * Example:
 * 
 * const char *a = "This is string a.";
 * const char *b = "This is string b.";
 * const char *ab = "This a different string.";
 * puts(CONCAT(a, b)); // Will print "This is a different string."
 * 
 * Parameters:
 *  - a: The left side of the concatenation
 *  - b: The right side of the concatenation
 * 
 * Returns: The concatenated literal
 */
#ifndef CONCAT
#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)
#endif

/* Error handling framework */

/*
 * spacepi_error_t represents an error code formatted by the spacepi library
 */
typedef union {
    /*
     * error_code is an integer form of the error code that can safely be set to errno (in theory).  It can then later
     * be deconstructed from errno back into this structure.
     */
    int error_code;
    /*
     * data represents the actual data fields inside the union that are equivalent to the error_code.
     */
    struct {
        /*
         * code is the specific error code for this error.
         */
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
        /*
         * type is the type of error (the source of error)
         */
        enum {
            system_err = 0, // It is a system error (defined in errno.h)
            spacepi = 1, // It is a spacepi error (defiend in spacepi_errors_t)
            mqtt = 2 // It is a MQTT error (defined in MQTTAsync.h)
        } type:2;
    } data;
} spacepi_error_t;

/*
 * spacepi_errors_t contains all of the errors that occur in the SpacePi library
 */
typedef enum {
    LIB_ALREADY_INIT = 0, // Library already initialized
    /*
     * SPACEPI_ERROR_CASCADE is not returned anywhere except when printing a stacktrace.  If you get one of these errors
     * returned out of a function, it is best just to call spacepi_perror() and then return the same error back again.
     * This way, the library can properly print the full stacktrace for the error.
     */
    SPACEPI_ERROR_CASCADE, // Cascaded error
    ALREADY_CONNECTED, // Already connected
    LIB_NOT_INIT, // Library not initialized
    UNKNOWN_ERROR, // Unknown error
    INVALID_PIN // Invalid pin
} spacepi_errors_t;

/*
 * spacepi_strerror converts an error structure into a string representation of the error.
 * 
 * Parameters:
 *  - spacepi_errno: The error to convert to a string
 * 
 * Returns: A string form of the error.  Do not modify the return value.  Subsequent calls to this function may change
 *          the returned value.
 */
const char *spacepi_strerror(spacepi_error_t spacepi_errno);
/*
 * spacepi_perror prints an error message to the console.  This function is also used with SPACEPI_ERROR_CASCADE to
 * print stack traces for crashes.  Normally instead of calling this directly, one would use the error checking macros
 * (see below).
 * 
 * Parameters:
 *  - func: The name of the function that returned the error (the function that was called)
 *  - file: The name of the file that the error occurred in (see __FILE__)
 *  - line: The line number that the error occurred in (see __LINE__)
 */
void spacepi_perror(const char *func, const char *file, int line);
/*
 * spacepi_trace prints a tracing message that can help debugging.  In general it is best not to leave any calls to this
 * function in the completed code.  Use TRACE() instead of directly calling this function.
 * 
 * Parameters:
 *  - file: The name of the file that is calling spacepi_trace (see __FILE__)
 *  - line: The line number which is calling spacepi_trace (see __LINE__)
 */
void spacepi_trace(const char *file, int line);

/*
 * The following are the error-handling macros and some quick documentation (more complete documentation is available
 * below):
 * 
 * Tracing:
 *   TRACE() - debug tracing
 * 
 * Error setting functions:
 *   Function                      | Error Type | Behavior
 *   ------------------------------+------------+----------
 *   SET_ERROR(type, val)          | Any        | None
 *   SET_ERROR_SYSTEM(val)         | errno.h    | None
 *   RETURN_ERROR(type, val)       | Any        | Return
 *   RETURN_ERROR_SYSTEM(val)      | errno.h    | Return
 *   JUMP_ERROR(label, type, var)  | Any        | Jump
 *   JUMP_ERROR_SYSTEM(label, val) | errno.h    | Jump
 *   RETURN_REPORTED_ERROR()       | Last       | Return
 * 
 *   Any = any type of error can be set
 *   errno.h = only errors defined in errno.h can be set
 *   Last = the last error that occurred
 *   None = control continues as normal
 *   Return = the current function returns and the calling function gets the error code
 *   Jump = the current function jumps to cleanup code that should call RETURN_REPORTED_ERROR() when done
 * 
 * Calling functions and checking errors:
 *   CHECK_ERROR(func, ...) - calls a function and returns if there was an error
 *   CHECK_ERROR_JUMP(label, func, ...) - calls a function and jumps to cleanup code if there was an error
 * 
 * Allocating memory:
 *   Function                                            | Definition | Type   | Behavior
 *   ----------------------------------------------------+------------+--------+----------
 *   CHECK_ALLOC(var, type)                              | Predefined | Scalar | Return
 *   CHECK_ALLOC_DEF(var, type)                          | Included   | Scalar | Return
 *   CHECK_ALLOC_ARRAY(var, type, count)                 | Predefined | Array  | Return
 *   CHECK_ALLOC_ARRAY_DEF(var, type, count)             | Included   | Array  | Return
 *   CHECK_ALLOC_JUMP(label, var, type)                  | Predefined | Scalar | Jump
 *   CHECK_ALLOC_DEF_JUMP(label, var, type)              | Included   | Scalar | Jump
 *   CHECK_ALLOC_ARRAY_JUMP(label, var, type, count)     | Predefined | Array  | Jump
 *   CHECK_ALLOC_ARRAY_DEF_JUMP(label, var, type, count) | Included   | Array  | Jump
 * 
 *   Predefined = variable must be declared before using the macro
 *   Incldued = variable definition is inside macro
 *   Scalar = single instance of object is allocated
 *   Array = multipe instances of object is allocated
 *   Return = If an error occurs, control returns out of this function
 *   Jump = If an error occurs, control jumps to cleanup code, which should call RETURN_REPORTED_ERROR() when done
 */

/*
 * TRACE prints a tracing message that can help debugging.  In general it is best not to leave any calls to this macro
 * in the completed code.
 */
#define TRACE() do { spacepi_trace(__FILE__, __LINE__); } while (0)
/*
 * SET_ERROR sets the current error that is occuring on the current thread to a specific value
 * 
 * Parameters:
 *  - type: The type (source system) of the error (see spacepi_error_t.data.type)
 *  - val: The specific error code (see spacepi_error_t.data.code)
 */
#define SET_ERROR(type, val) do { errno = ((spacepi_error_t) { .data = { CONCAT(.ty, pe) = type, .code = val }}).error_code; } while (0)
/*
 * SET_ERROR_SYSTEM sets the current error that is occuring on the current thread to a specific value defined in errno.h
 * 
 * Parameters:
 *  - val: The specific error code (see spacepi_error_t.data.code)
 */
#define SET_ERROR_SYSTEM(val) SET_ERROR(system_err, val)
/*
 * RETURN_ERROR sets the current error and then returns it (a call to this macro causes the current function to return).
 * 
 * Parameters:
 *  - type: The type (source system) of the error (see spacepi_error_t.data.type)
 *  - val: The specific error code (see spacepi_error_t.data.code)
 */
#define RETURN_ERROR(type, val) do { SET_ERROR(type, val); return -1; } while (0)
/*
 * RETURN_ERROR_SYSTEM sets the current error to a specific value defined in errno.h and then returns it (a call to this
 * macro causes the current function to return).
 * 
 * Parameters:
 *  - val: The specific error code (see spacepi_error_t.data.code)
 */
#define RETURN_ERROR_SYSTEM(val) RETURN_ERROR(system_err, val)
/*
 * RETURN_REPORTED_ERROR returns an error that was already captured with CHECK_ERROR, CHECK_ALLOC, CHECK_ALLOC_DEF,
 * CHECK_ALLOC_ARRAY, CHECK_ALLOC_ARRAY_DEF, CHECK_ERROR_JUMP, CHECK_ALLOC_JUMP, CHECK_ALLOC_DEF_JUMP,
 * CHECK_ALLOC_ARRAY_JUMP, CHECK_ALLOC_ARRAY_DEF_JUMP and prints the stacktrace (incrementally).  At the end of any
 * cleanup code run by any of those _JUMP functions, this should be called to return the error to the parent function.
 */
#define RETURN_REPORTED_ERROR() do { SET_ERROR(spacepi, SPACEPI_ERROR_CASCADE); return -1; } while (0)
/*
 * CHECK_ERROR runs a function call and checks it for any errors that may have occurred.  If an error did occur, it
 * prints the stacktrace and returns from the current function with an error code.
 * 
 * Parameters:
 *  - func: The name of a function that implements SpacePi's error handling convention
 *  - ...: The rest of the arguments are the arguments to the function
 */
#define CHECK_ERROR(func, ...) do { if (func(__VA_ARGS__) < 0) { spacepi_perror(STR(func), __FILE__, __LINE__); RETURN_REPORTED_ERROR(); } } while (0)
/*
 * CHECK_ALLOC allocates dynamic memory and ensures that there was space to allocate it.  If there was not space left to
 * allocate the memory, an error message is printed along with the stack trace, and the current function returns the
 * error.
 * 
 * Parameters:
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 */
#define CHECK_ALLOC(var, type) do { var = (type *) malloc(sizeof(type)); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); RETURN_ERROR_SYSTEM(ENOBUFS); } } while (0)
/*
 * CHECK_ALLOC_DEF defines a variable and allocates dynamic memory for it, while ensuring that there was space to
 * allocate it.  If there was not space left to allocate the memory, an error message is printed along with the stack
 * trace, and the current function returns the error.
 * 
 * Parameters:
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 */
#define CHECK_ALLOC_DEF(var, type) type *var; CHECK_ALLOC(var, type)
/*
 * CHECK_ALLOC_ARRAY allocates dynamic memory for an array and ensures that there was space to allocate it.  If there
 * was not space left to allocate the memory, an error message is printed along with the stack trace, and the current
 * function returns the error.
 * 
 * Parameters:
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 *  - count: The number of elements in the array to be allocated
 */
#define CHECK_ALLOC_ARRAY(var, type, count) do { var = (type *) malloc(sizeof(type) * (count)); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); RETURN_ERROR_SYSTEM(ENOBUFS); } } while (0)
/*
 * CHECK_ALLOC_ARRAY_DEF defines a variable and allocates dynamic memory for an array, while ensuring that there was
 * space to allocate it.  If there was not space left to allocate the memory, an error message is printed along with the
 * stack trace, and the current function returns the error.
 * 
 * Parameters:
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 *  - count: The number of elements in the array to be allocated
 */
#define CHECK_ALLOC_ARRAY_DEF(var, type, count) var *type; CHECK_ALLOC_ARRAY(var, type, count)
/*
 * JUMP_ERROR sets the current error that is occuring on the current thread to a specific value, then jumps to some
 * error handling code.  After error handling is complete, the function should call RETURN_REPORTED_ERROR() to notify
 * the calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - type: The type (source system) of the error (see spacepi_error_t.data.type)
 *  - val: The specific error code (see spacepi_error_t.data.code)
 */
#define JUMP_ERROR(label, type, val) do { SET_ERROR(type, val); goto label; } while (0)
/*
 * JUMP_ERROR_SYSTEM sets the current error that is occuring on the current thread to a specific value defined in
 * errno.h, then jumps to some error handling code.  After error handling is complete, the function should call
 * RETURN_REPORTED_ERROR() to notify the calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - val: The specific error code (see spacepi_error_t.data.code)
 */
#define JUMP_ERROR_SYSTEM(label, val) JUMP_ERROR(label, system_err, val)
/*
 * CHECK_ERROR_JUMP runs a function call and checks it for any errors that may have occurred.  If an error did occur, it
 * prints the stacktrace and jumps to some error handling code.  After error handling is complete, the function should
 * call RETURN_REPORTED_ERROR() to notify the calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - func: The name of a function that implements SpacePi's error handling convention
 *  - ...: The rest of the arguments are the arguments to the function
 */
#define CHECK_ERROR_JUMP(label, func, ...) do { if (func(__VA_ARGS__) < 0) { spacepi_perror(STR(func), __FILE__, __LINE__); goto label; } } while (0)
/*
 * CHECK_ALLOC_JUMP allocates dynamic memory and ensures that there was space to allocate it.  If there was not space
 * left to allocate the memory, an error message is printed along with the stack trace, and it jumps to some error
 * handling code.  After error handling is complete, the function should call RETURN_REPORTED_ERROR() to notify the
 * calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 */
#define CHECK_ALLOC_JUMP(label, var, type) do { var = (type *) malloc(sizeof(type)); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); JUMP_ERROR_SYSTEM(label, ENOBUFS); } } while (0)
/*
 * CHECK_ALLOC_DEF defines a variable and allocates dynamic memory for it, while ensuring that there was space to
 * allocate it.  If there was not space left to allocate the memory, an error message is printed along with the stack
 * trace, and it jumps to some error handling code.  After error handling is complete, the function should call
 * RETURN_REPORTED_ERROR() to notify the calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 */
#define CHECK_ALLOC_DEF_JUMP(label, var, type) type *var; CHECK_ALLOC_JUMP(label, var, type)
/*
 * CHECK_ALLOC_ARRAY_JUMP allocates dynamic memory for an array and ensures that there was space to allocate it.  If
 * there was not space left to allocate the memory, an error message is printed along with the stack trace, and it jumps
 * to some error handling code.  After error handling is complete, the function should call RETURN_REPORTED_ERROR() to
 * notify the calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 *  - count: The number of elements in the array to be allocated
 */
#define CHECK_ALLOC_ARRAY_JUMP(label, var, type, count) do { var = (type *) malloc(sizeof(type) * count); if (!var) { fflush(stdout); fputs("malloc: No buffer space available (" __FILE__ ":" STR(__LINE__) ")\n", stderr); JUMP_ERROR_SYSTEM(label, ENOBUFS); } } while (0)
/*
 * CHECK_ALLOC_ARRAY_DEF_JUMP defines a variable and allocates dynamic memory for an array, while ensuring that there
 * was space to allocate it.  If there was not space left to allocate the memory, an error message is printed along with
 * the stack trace, and it jumps to some error handling code.  After error handling is complete, the function should
 * call RETURN_REPORTED_ERROR() to notify the calling functions that the stack trace needs to be printed.
 * 
 * Parameters:
 *  - label: The label to jump to in order to clean up local resources
 *  - var: The name of the variable to allocate
 *  - type: The type of variable that should be allocated (var should be a pointer to this type)
 *  - count: The number of elements in the array to be allocated
 */
#define CHECK_ALLOC_ARRAY_DEF_JUMP(label, var, type, count) type *var; CHECK_ALLOC_ARRAY_JUMP(label, var, type, count)

/* Pub/sub framework */

/*
 * spacepi_qos_t denotes the importance of information contained in the packet.
 */
typedef enum {
    /*
     * at_most_once sends it once and if it doesn't arrive it won't be resent.  This is useful for messages coming from
     * sensors that update so fast that resending a packet would only result in old data being received.  This mode is
     * the least computationally expensive.
     */
    at_most_once = 0,
    /*
     * at_least_once sends it as many times as it must to make sure that it is received at least once time.  This is
     * useful for messages that can be duplicated without having any effect on the system, but must be delivered at
     * least one time, like a message that says a specific pyro charge should light.  If the pyro charge was lit twice,
     * there would be no ill effects, but the message does need to arrive at least once.
     */
    at_least_once = 1,
    /*
     * exactly_once makes sure that it is received exactly once time.  This is useful for messages that much be
     * delivered and duplicate messages cause problems.  An example of this would be a command to move a stepper,
     * because it must be delivered at least once to get it to move, and if using relative offsets, delivering the
     * message multiple times would cause the stepper to move too much.  This mode is the most computationally
     * expensive.
     */
    exactly_once = 2
} spacepi_qos_t;

/*
 * spacepi_pubsub_connection_t represents the current connection state to the MQTT server
 */
typedef enum {
    disconnected = 0, // This program is not connected to the server
    connected = 1, // This program is connected to the server
    connecting = 2 // This program is currently trying to connect to the server
} spacepi_pubsub_connection_t;

/*
 * spacepi_token_t represents a token that represents some task.  This token can then be given to spacepi_wait_token to
 * block until the task this token represents finishes.
 */
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
 * // Define a new function, `add_fund`, that adds a function that takes two ints to the thread pool
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
 *  - n: The number of arguments
 *  - func_name: The name of the function to write
 *  - ...: The argument types for the function
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

/* IO Functions */

#ifdef LOW
#if LOW != 0
#error LOW != 0
#endif
#else
#define LOW 0
#endif
#ifdef HIGH
#if HIGH != 1
#error HIGH != 1
#endif
#else
#define HIGH 1
#endif

/*
 * pin_t represents a pin that is connected to the computer
 */
typedef struct {
    /*
     * controller is the name of the driver that controls this pin
     */
    const char *controller;
    /*
     * address is the address of the hardware that controls this pin
     */
    unsigned address;
    /*
     * pin is the pin number on the controlling device
     */
    unsigned pin;
    /*
     * driver is a pointer to custom driver data that should not be used by users of this library
     */
    void *driver;
} pin_t;

/*
 * pin_mode_t represents the different modes a pin can be in
 */
typedef enum {
    output = 0, // The pin is an output pin
    input_hi_z = 1, // The pin is an input pin with no pull ups or pull downs
    input_pullup = 3, // The pin is an input pin with a pullup resistor enabled
    input_pulldown = 5 // The pin is an input pin with a pulldown resistor enabled
} pin_mode_t;

/*
 * edge_t represents the edge of a waveform
 */
typedef enum {
    none = 0, // No edge
    rising = 1, // The rising edge (transition from LOW to HIGH)
    falling = 2, // The falling edge (transition from HIGH to LOW)
    both = 3 // Either edge (any transition between LOW and HIGH)
} edge_t;

/*
 * get_pin gets a pin instance from its name.  The name should be in the form of
 * <bus><pin>, <bus>:<pin>, or <bus>@<address>:<pin>
 * 
 * Parameters:
 *  - name: The name of the pin
 *  - pin: The variable to store the pin in
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int get_pin(const char *name, pin_t *pin);
/*
 * pin_mode sets the mode of a pin
 * 
 * Parameters:
 *  - pin: The pin to set
 *  - mode: The mode to set the pin to
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int pin_mode(pin_t *pin, pin_mode_t mode);
/*
 * digital_write writes a value to a pin
 * 
 * Parameters:
 *  - pin: The pin to write to
 *  - value: The value the pin should be
 *
 * Return: 0 on success or a negative number with errno set on error
 */
int digital_write(pin_t *pin, int value);
/*
 * digital_read reads a value from a pin
 * 
 * Parameters:
 *  - pin: The pin to write to
 * 
 * Return: HIGH or LOW, or a negative number with errno set on error
 */
int digital_read(pin_t *pin);
/*
 * attach_interrupt registers an interrupt handler for a pin to be called every time it changes value
 * 
 * Parameters:
 *  - pin: The pin to register the interrupt on
 *  - edge: The edge that should trigger an interrupt
 *  - callback: The function to call when an interrupt is received
 *  - context: A context object to give to the callback
 * 
 * Return: 0 on success or a negative number with errno set on error
 */
int attach_interrupt(pin_t *pin, edge_t edge, void (*callback)(void *context), void *context);

#endif
