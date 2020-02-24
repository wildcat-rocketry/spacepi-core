#ifndef TIMING_H
#define TIMING_H

#include <time.h>

extern const struct timespec delay_between_scans;
extern const struct timespec delay_between_prints;
extern const struct timespec encoding_one_high;
extern const struct timespec encoding_zero_high;
extern const struct timespec encoding_period;

#endif
