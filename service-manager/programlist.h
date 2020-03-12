#ifndef PROGRAM_LIST_H
#define PROGRAM_LIST_H

#include <stdio.h>
#include "program.h"
#include "resolver.h"

struct _program_list;
typedef struct _program_list program_list_t;
struct _program_list {
    program_t program;
    int enabled; // Programs can be disabled by putting a '!' before their name in the list file
    program_list_t *next;
};

int program_list_read(FILE *file, const resolver_t *resolver, program_list_t **list);
int program_list_free(program_list_t *list);
int program_list_watch(program_list_t *list);

#endif
