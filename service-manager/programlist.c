#include <stdio.h>
#include "program.h"
#include "resolver.h"
#include "programlist.h"

int program_list_read(FILE *file, const resolver_t *resolver, program_list_t **list) {
    // TODO
    return 0;
}

int program_list_free(program_list_t *list) {
    // TODO
    return 0;
}

int program_list_watch(program_list_t *list) {
    // TODO
    // Register a handler for SIGCHLD in this function, then in that handler
    // check through the list to figure out which process died, then call its
    // on_death callback
    return 0;
}
