#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <spacepi.h>
#include "ips.h"

int main(int argc, const char **argv) {
    CHECK_ERROR(ips_reset);
    uint32_t addr;
    while (ips_next(&addr)) {
        printf("%d.%d.%d.%d\n", (addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, (addr >> 0) & 0xFF);
    }
    return EXIT_SUCCESS;
}
