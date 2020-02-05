#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <spacepi.h>
#include "ips.h"

int main_loop(void);
void exit_handler(int sig);

int main(int argc, const char **argv) {
    signal(SIGINT, exit_handler);
    main_loop();
    return EXIT_FAILURE;
}

int main_loop(void) {
    CHECK_ERROR(ips_reset);
    uint32_t addr;
    while (ips_next(&addr)) {
        printf("%d.%d.%d.%d\n", (addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, (addr >> 0) & 0xFF);
    }
}

void exit_handler(int sig) {
    uint32_t addr;
    while (ips_next(&addr));
    exit(EXIT_SUCCESS);
}
