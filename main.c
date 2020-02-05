#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <spacepi.h>
#include "encoder.h"
#include "ips.h"
#include "led.h"
#include "timing.h"

int main_loop(void);
void exit_handler(int sig);

int main(int argc, const char **argv) {
    signal(SIGINT, exit_handler);
    main_loop();
    return EXIT_FAILURE;
}

int main_loop(void) {
    CHECK_ERROR(led_init);
    struct timespec timer;
    CHECK_ERROR(timer_init, &timer);
    while (TRUE) {
        CHECK_ERROR(ips_reset);
        uint32_t addr;
        while (ips_next(&addr)) {
            CHECK_ERROR(encode, addr, &timer);
            CHECK_ERROR(sleep_to, &timer, delay_between_prints);
        }
        CHECK_ERROR(sleep_to, &timer, delay_between_scans);
    }
}

void exit_handler(int sig) {
    uint32_t addr;
    while (ips_next(&addr));
    exit(EXIT_SUCCESS);
}
