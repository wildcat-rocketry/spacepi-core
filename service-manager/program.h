#ifndef PROGRAM_H
#define PROGRAM_H

#include <time.h>
#include <linux/limits.h>
#include <sys/types.h>

struct _program_t;
typedef struct _program_t program_t;

typedef void (*death_handler_t)(program_t *program);

struct _program_t {
    pid_t pid;
    const char filename[PATH_MAX];
    char *const *args;
    death_handler_t on_death; // Function to call when the process dies
    // File descriptors from pipe
    int stdout[2];
    int stderr[2];
    // Used to detect when it is useless to keep respawning a process
    int times_started;
    struct timespec last_start;
};

int program_init(program_t *program);
int program_start(program_t *program);
int program_is_alive(const program_t *program);

#endif