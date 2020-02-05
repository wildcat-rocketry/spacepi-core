#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <spacepi.h>
#include "ips.h"
#include "timing.h"

int main_loop(void);
void exit_handler(int sig);

int main(int argc, const char **argv) {
    signal(SIGINT, exit_handler);
    main_loop();
    return EXIT_FAILURE;
}

int main_loop(void) {
    struct timespec last_time;
    struct timespec now;
    CHECK_ERROR(clock_gettime, CLOCK_REALTIME, &last_time);
    while (TRUE) {
        CHECK_ERROR(clock_gettime, CLOCK_REALTIME, &now);
        if (last_time.tv_nsec + BETWEEN_SCANS_NS < last_time.tv_nsec || last_time.tv_nsec + BETWEEN_SCANS_NS >= 1000000000L) {
            last_time.tv_nsec += BETWEEN_SCANS_NS - 1000000000L;
            ++last_time.tv_sec;
        } else {
            last_time.tv_nsec += BETWEEN_SCANS_NS;
        }
        last_time.tv_sec += BETWEEN_SCANS_S;
        if (last_time.tv_nsec - now.tv_nsec < 0 || last_time.tv_nsec - now.tv_nsec > last_time.tv_nsec) {
            now.tv_nsec = last_time.tv_nsec + 1000000000L - now.tv_nsec;
            now.tv_sec = last_time.tv_sec - 1 - now.tv_sec;
        } else {
            now.tv_nsec = last_time.tv_nsec - now.tv_nsec;
            now.tv_sec = last_time.tv_sec - now.tv_sec;
        }
        CHECK_ERROR(nanosleep, &now, NULL);
        CHECK_ERROR(ips_reset);
        uint32_t addr;
        while (ips_next(&addr)) {
            printf("%d.%d.%d.%d\n", (addr >> 24) & 0xFF, (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, (addr >> 0) & 0xFF);
        }
    }
}

void exit_handler(int sig) {
    uint32_t addr;
    while (ips_next(&addr));
    exit(EXIT_SUCCESS);
}
