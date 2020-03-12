#ifndef LOGGER_H
#define LOGGER_H

#include <linux/limits.h>
#include <pthread.h>

typedef struct {
    char program_name[PATH_MAX];
    int instance_id; // Index in the program list file
    int stdout_pipe;
    int stderr_pipe;
    int log_file;
} logger_t;

typedef struct {
    logger_t *loggers;
    int num_loggers;
    int thread_pipe[2]; // Pipe for communicating to the reading thread
    pthread_t thread;
} loggers_t;

int loggers_init(loggers_t *loggers);
int logger_set_pipes(loggers_t *loggers, logger_t *logger, int stdout_pipe, int stderr_pipe);
int loggers_free(loggers_t *loggers);

#endif
