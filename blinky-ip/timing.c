#include <stdint.h>
#include <time.h>
#include <spacepi.h>
#include "timing.h"

const struct timespec delay_between_scans = {
    .tv_sec = 10,
    .tv_nsec = 0
};

const struct timespec delay_between_prints = {
    .tv_sec = 1,
    .tv_nsec = 0
};

const struct timespec encoding_one_high = {
    .tv_sec = 0,
    .tv_nsec = 800000000
};

const struct timespec encoding_zero_high = {
    .tv_sec = 0,
    .tv_nsec = 200000000
};

const struct timespec encoding_period = {
    .tv_sec = 1,
    .tv_nsec = 0
};

int timer_init(struct timespec *timer) {
    CHECK_ERROR(clock_gettime, CLOCK_REALTIME, timer);
    return 0;
}

int sleep_to(struct timespec *timer, struct timespec delay) {
    struct timespec now;
    CHECK_ERROR(timer_init, &now);
    if (timer->tv_nsec + delay.tv_nsec < timer->tv_nsec || timer->tv_nsec + delay.tv_nsec >= 1000000000L) {
        timer->tv_nsec += delay.tv_nsec - 1000000000L;
        ++timer->tv_sec;
    } else {
        timer->tv_nsec += delay.tv_nsec;
    }
    timer->tv_sec += delay.tv_sec;
    if (timer->tv_nsec - now.tv_nsec < 0 || timer->tv_nsec - now.tv_nsec > timer->tv_nsec) {
        now.tv_nsec = timer->tv_nsec + 1000000000L - now.tv_nsec;
        now.tv_sec = timer->tv_sec - 1 - now.tv_sec;
    } else {
        now.tv_nsec = timer->tv_nsec - now.tv_nsec;
        now.tv_sec = timer->tv_sec - now.tv_sec;
    }
    CHECK_ERROR(nanosleep, &now, NULL);
    return 0;
}
