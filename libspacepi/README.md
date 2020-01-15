# libspacepi

The libspacepi library is separated in (partially) independent subsystems.
Here is a list of subsystems along with their documentation:

 * [Booleans](#booleans)
 * [Various Macros](#various-macros)
 * [Error Handling](#error-handling)
 * [Publish/Subscribe Framework](#publishsubscribe-framework)
 * [Random Number Generation](#random-number-generation)
 * [Thread Pooling Framework](#thread-pooling-framework)
 * [Hardware IO Framework](#hardware-io-framework)

## Booleans

Booleans are represented as normal `int`s in the library.
A `TRUE` is represented as a 1, and a `FALSE` is represented as a 0.
The constants `TRUE` and `FALSE` are also defined, and their use is preferred over the numerical literals.
Since C treats any integer besides 0 as true and 0 as false, this allows `if` statements to treat `TRUE` and `FALSE` like other languages do.

## Various Macros

These are macros that are used in other parts of the `spacepi.h` header file, and it is likely that when using the library you will not need to use any of them.

### STR(x)

STR converts literal text to a string.  For example, STR(abc) is replace with "abc" during preprocessing.
**NOTE:** you use this with a variable name, it will create a string that contains the name of the variable.

#### Example

```c
puts(STR(Hello, world!));
// prints "Hello, world!"
```

### CONCAT(a, b)

CONCAT concatenates two sections of literal text and forms a new literal (can be used for identifiers).
**NOTE**: if you use this with two variable names, it will concatenate the variable names instead of the values of the variables.

#### Example

```c
const char *a = "This is string a.";
const char *b = "This is string b.";
const char *ab = "This a different string.";
puts(CONCAT(a, b));
// prints "This is a different string."
```

## Error Handling

### Error Passing Convention

You do not need to understand the convention to use the library.
This section describes how the library works, so if you wish to extend the library, then understanding this section can be helpful.

In order to convey errors that occur, some functions will return `-1` and set `errno` to an error code.
This library extends that functionality by adding support for custom error codes.
The most significant few bits of `errno` are used for identifying the source of the error, and the rest of the bits are used for the error code.
This way, the library originating the error can be detected and all code can pass errors in a uniform way.

To print stack traces, a new error called `SPACEPI_ERROR_CASCADE` was introduced.
This error does is not returned from any failing function calls, but instead returned from functions that have already handled the error, but are printing a stacktrace.
After a function errors, the first function handles that error and prints error information, then errors with the `SPACEPI_ERROR_CASCADE` code.
Functions further back in the stack then catch that `SPACEPI_ERROR_CASCADE` code and print their filename and line number to the console, then pass the error back again.
This way, the error gets printed, and the stack trace also gets printed.

### Backing Functions

These three functions form the basis for all of the error detection macros.
In general, you will not need to call these functions, but there may be some times in which it is better to use the functions directly instead of using the macros.
It would be very difficult to write macros for every possible case in which errors may occur.

#### File Names and Line Numbers

These functions take file names and line numbers as arguments.
Instead of hardcoding these, please use `__FILE__` and `__LINE__` so that when the file is changed the values update.
Otherwise, it will become very difficult to debug when the line numbers don't line up with what they should be.
All the macros automatically do this for you.
Also, if you put `__FILE__` or `__LINE__` in a macro, it will replace it will the filename and line number where the macro is called, not defined (which is the desired behavior most of the time).

#### spacepi_strerror

This is identical to the `strerror` function from `string.h`, except that it works with any errors following the SpacePi Error Passing Conventions.

#### spacepi_perror

This is very similar to the `perror` function from `stdio.h`, except that it works with any errors following the SpacePi Error Passing Conventions and it also prints out stacktrace information.
It is recommended that you call this function immediately after detecting an error (which is normally handled through one of the macros).
After that, it should be called in each stack frame with the `SPACEPI_ERROR_CASCADE` error to print the rest of the stacktrace.

#### spacepi_trace

This just prints a simple debugging message for tracing.
Please use this instead of randomly printing "here" to the console so it is more tracable.
In almost every case, the `TRACE()` macro is preferred over this function because it will automatically use the `__FILE__` and `__LINE__` macros.

### Error Handling Macros

Instead of writing tons of code to detect errors and handle them with the three above functions, a series of macros have been written that do a lot of it for you.


#### Tracing Macros

The only tracing macro is `TRACE()` (see [spacepi_trace](#spacepitrace) above).
This just prints a simple debugging message for tracing.
Please use this instead of randomly printing "here" to the console so it is more tracable.

#### Error Causing Functions

Function                      | Error Type | Behavior
------------------------------|------------|----------
SET_ERROR(type, val)          | Any        | None
SET_ERROR_SYSTEM(val)         | errno.h    | None
RETURN_ERROR(type, val)       | Any        | Return
RETURN_ERROR_SYSTEM(val)      | errno.h    | Return
JUMP_ERROR(label, type, var)  | Any        | Jump
JUMP_ERROR_SYSTEM(label, val) | errno.h    | Jump
RETURN_REPORTED_ERROR()       | Last       | Return

_Any_ = any type of error can be set

_errno.h_ = only errors defined in errno.h can be set

_Last_ = the last error that occurred

_None_ = control continues as normal

_Return_ = the current function returns and the calling function gets the error code

_Jump_ = the current function jumps to cleanup code that should call `RETURN_REPORTED_ERROR()` when done

#### Calling Functions and Checking Errors

`CHECK_ERROR(func, ...)` - calls a function and returns if there was an error

`CHECK_ERROR_JUMP(label, func, ...)` - calls a function and jumps to cleanup code if there was an error

#### Allocating Memory

Function                                            | Definition | Type   | Behavior
----------------------------------------------------|------------|--------|----------
CHECK_ALLOC(var, type)                              | Predefined | Scalar | Return
CHECK_ALLOC_DEF(var, type)                          | Included   | Scalar | Return
CHECK_ALLOC_ARRAY(var, type, count)                 | Predefined | Array  | Return
CHECK_ALLOC_ARRAY_DEF(var, type, count)             | Included   | Array  | Return
CHECK_ALLOC_JUMP(label, var, type)                  | Predefined | Scalar | Jump
CHECK_ALLOC_DEF_JUMP(label, var, type)              | Included   | Scalar | Jump
CHECK_ALLOC_ARRAY_JUMP(label, var, type, count)     | Predefined | Array  | Jump
CHECK_ALLOC_ARRAY_DEF_JUMP(label, var, type, count) | Included   | Array  | Jump

_Predefined_ = variable must be declared before using the macro

_Incldued_ = variable definition is inside macro

_Scalar_ = single instance of object is allocated

_Array_ = multipe instances of object is allocated

_Return_ = If an error occurs, control returns out of this function

_Jump_ = If an error occurs, control jumps to cleanup code, which should call `RETURN_REPORTED_ERROR()` when done

#### Examples

```c
/* Examples of backing functions */

SET_ERROR(st_spacepi, LIB_ALREADY_INIT);
const char *str = spacepi_strerror(errno);
puts(str); // Library already initialized

SET_ERROR(st_spacepi, LIB_ALREADY_INIT);
spacepi_perror("my_func", __FILE__, __LINE__);
// my_func: Library already initialized (0x40000000)
//   at main.c:123
SET_ERROR(st_spacepi, SPACEPI_ERROR_CASCADE);
spacepi_perror("my_other_func", __FILE__, __LINE__);
//   at main.c:456

spacepi_trace(__FILE__, __LINE__);
// (Trace at main.c:128)

/* Examples of macros */

int my_func(void) {
    int err = TRUE; // Set error to true because if control diverges an error must've occurred

    if (some_argument_is_wrong) {
        RETURN_ERROR_SYSTEM(EINVAL);
    }
    CHECK_ERROR(some_function, "arg1", "arg2");
    void *obj = allocate_something();

    // creates a new `char *buffer`
    // allocates 4096 chars in the buffer
    // if there is a failure, it jumps down to free_obj
    CHECK_ALLOC_ARRAY_DEF_JUMP(free_obj, buffer, char, 4096);

    // do_something("args", buffer, obj);
    // if error, goto free_buffer
    CHECK_ERROR_JUMP(free_buffer, do_something, "args", buffer, obj);

    err = FALSE; // If control hasn't diverged until here, we must not've had any errors

    // Clean up resources used by the function
    free_buffer:
    free(buffer);
    // after free_buffer runs, free_obj runs since it is in the code after it
    free_obj:
    free(obj);
    // following this structure allows you to free any resources you've allocated, but not any you haven't gotten to yet (if buffer fails to allocate, it'll only run from `free_obj:` on)

    if (err) { // err is only true if control diverged
        RETURN_HANDLED_ERROR(); // Keep printing the stacktrace and tell the function that called this one there's an error
    } else {
        return 0; // 0 = no error
    }
}

// my_func();
// if error from RETURN_ERROR_SYSTEM, print it out
// if error that was handled, just continue printing the stacktrace
// if there was any error, return control to the calling function
CHECK_ERROR(my_func);
```

## Publish/Subscribe Framework

In order to pass messages between processes and hosts, this publish/subscribe framework is used.
One process will publish data to a channel, then other processes can subscribe to that process in order to receive that data transmission.
This is run over MQTT, so if you would like more information, search about MQTT on the Internet.

### Decoding Packets

A lot of data packets contain multiple data types, so a convinient way to decode these are as follows:

```c
typedef struct __attribute__((__packed__)) {
    float arg1;
    int arg2;
    uint16_t etc;
} my_data_packet_t;

const my_data_packet_t *decode_my_data_packet(const void *data, size_t data_len) {
    if (data_len < sizeof(my_data_packet_t)) {
        return NULL; // error
    }
    return (const my_data_packet_t *) data;
}
```

### QoS

There are three different QoS settings that packets can have.

1. At most once: Sends it once and if it doesn't arrive it won't be resent.
                 This is useful for messages coming from sensors that update so fast that resending a packet would only result in old data being received.
                 This mode is the least computationally expensive.
2. At least once: Sends it as many times as it must to make sure that it is received at least once time.
                  This is useful for messages that can be duplicated without having any effect on the system, but must be delivered at least one time, like a message that says a specific pyro charge should light.
                  If the pyro charge was lit twice, there would be no ill effects, but the message does need to arrive at least once.
3. Exactly once: Makes sure that it is received exactly once time.
                 This is useful for messages that much be delivered and duplicate messages cause problems.
                 An example of this would be a command to move a stepper, because it must be delivered at least once to get it to move, and if using relative offsets, delivering the message multiple times would cause the stepper to move too much.
                 This mode is the most computationally expensive.

#### spacepi_pubsub_init()

This function must be called before any other function in the pubsub framework can be called.

### spacepi_pubsub_cleanup()

This function cleans up all resources used by the pubsub framework.
Cleanup is automatically run when returning from `main()` or calling `exit()`.
The main use for calling this directly would be if wanting to reinitialize the library (you can't call `spacepi_pubsub_init` twice without calling `spacepi_pubsub_cleanup` in between).

### spacepi_pubsub_connected(), spacepi_pubsub_connection_handler()

These two functions are for determining when the library is connected to the server.
Normally you would not need to use these, though.
If you, for example, try to publish while the library is reconnecting to the server (which is an automatic process), the publish function will block until the connection is established.

### spacepi_handle_server_down()

This sets a callback to be called if the library is unable to reconnect to the server.
The default behavior (if you don't set your own) is to quit the program.
This is also called if the first connection attempt fails.

### spacepi_publish(), spacepi_publish_token(), spacepi_publish_callback(), spacepi_wait_token()

These functions publish a message to the messaging bus.
Other processes can receive these messages when they subscribe to the same channel.

By default, all clients that are currently subscribed to the channel will receive the message.
If `retain` is set to `TRUE`, the message will additionally be received by any new clients that subscribe to the channel at a later time (until another message with `retain` set to `TRUE` is published).

`spacepi_publish` is the most basic form of the call.

`spacepi_publish_token` returns a token that can later be given to `spacepi_wait_token` to block until the publish command is finished.

`spacepi_publish_callback` calls another function after the publish command is finished.

### spacepi_subscribe(), spacepi_unsubscribe()

These functions manage the subscriptions the process has.
Each subscription also has a function associated it that is called when a new packet arrives with that channel.

Subscriptions can also contain wildcards (publications cannot) in their channel names.
The `#` character may be used at the end of a channel name to denote that any number of characters following it match.
The special case of this is when the channel name itself is just `"#"`, in which case it subscribes to all messages.
The `:` character may be used to denote any number of characters between two `/`s. (ex. `/abc/#`, `/abc/:/123`, etc.)

## Random Number Generation

### random_bytes()

Fills a buffer with random bytes.

### random_byte

Gets one random byte (value [0, 256)).

### randomize_string()

Creates a random string.

## Thread Pooling Framework

The thread pool provides a way to run callbacks without blocking until the callback finishes.
Any function can be enqueued onto the thread pool, and that function will shortly be called on a different thread.
Threads are automatically created and destroyed based on how heavily a program uses the thread pool.

### thread_pool_init()

Sets up the thread pool.

### thread_enqueue()

This is the low-level function for adding a new entry to the thread pooling queue.
Instead of calling this, use the thread enqueueing macros.

### thread_block()

This function blocks the current thread forever so it does not die.
This never returns once you call it, so do not call it unless you really mean to.
One example of when you need this is to keep the main thread from dying while other threads are still running.

### thread_yield()

This function blocks for a variable amount of time to help reduce CPU load when polling something.
It takes one parameter as the maximum amount of time to block for, which makes sure that the data is polled frequently enough.
Other than that, it will increase the polling frequency if there is low system load, and reduce the frequency if there is high system load.

### Macros

There are macros defined that write a function to enqueue a new function in the thread pool.
This way, you can call functions that have any arguments instead of relying on only the overloads of `thread_enqueue`.

#### Example

```c
#include <stdio.h>
#include <spacepi.h>

void my_callback(int a, int b);

// Define a new function, `add_fund`, that adds a function that takes two ints to the thread pool
DEFINE_THREAD_ENQUEUE_2(add_func, int, int);

int main() {
    // Using the error framework
    CHECK_ERROR(thread_pool_init);
    CHECK_ERROR(add_func, my_callback, 2, 4);

    // Not using the error framework
    thread_pool_init();
    add_func(my_callback, 2, 4);

    return 0;
}

void my_callback(int a, int b) {
    printf("a = %d, b = %d\n", a, b);
}

// Prints out "a = 2, b = 4"
```

## Hardware IO Framework

The hardware IO framework gives the program access to GPIO pins.
In addition, pins connected through other controllers, like IO expanders, are also available.

### Pin Name Format

Each pin name contains information about the driver that controls access to the hardware, the pin number, and possibly an address the driver needs.
Those three can be encoded in any of the following formats:

1. `<driver>@<address>:<pin>`
2. `<driver>:<pin>`
3. `<driver><pin>`
4. `<driver>`

NOTE: a `!` or `~` character can be prepended to a pin name to make it logically inverted.

The driver name is an ASCII string, the address is encoded in hexadecimal ASCII, and the pin number is encoded in decimal ASCII.
Programs should not encode or decode these strings, but instead rely on the user to encode them and the SpacePi library to decode them.

### get_pin()

This decodes the pin name and initializes the driver.
It also populates a `pin_t` structure that all of the other calls use as an argument.

### pin_mode()

This sets up the pin to be either an `sm_output` pin, or an input with no pull resistors (`sm_input_hi_z`), pull down resistors (`sm_input_pulldown`), or pull up resistors (`sm_input_pullup`).
Some drivers or hardware may not implement this function.

### digital_write(), digital_read()

These either read from or write to a pin.
Some drivers or hardware may not implement these functions.

### attach_interrupt()

This attaches an interrupt handler that is called on either the `si_rising` edge, the `si_falling` edge, or `si_both`.
Some drivers or hardware may not implement this function.
