#include "logger.h"

int loggers_init(loggers_t *loggers) {
    // TODO
    // Spawn one thread that will read from all of the loggers
    return 0;
}

int logger_set_pipes(loggers_t *loggers, logger_t *logger, int stdout_pipe, int stderr_pipe) {
    // TODO
    // Notify the logging thread that one of the pipes is changing
    return 0;
}

int loggers_free(loggers_t *loggers) {
    // TODO
    // Don't forget to clean up the other thread too
    return 0;
}
