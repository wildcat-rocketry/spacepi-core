#include <time.h>
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
