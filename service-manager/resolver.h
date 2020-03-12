#ifndef RESOLVER_H
#define RESOLVER_H

#include <linux/limits.h>
#include "config.h"

typedef struct {
    program_mode_t mode;
    char baseDir[PATH_MAX];
} resolver_t;

int resolver_init(resolver_t *resolver, const config_t *config);
int resolve_filename(const resolver_t *resolver, char *filename, const char *program_name);

#endif
