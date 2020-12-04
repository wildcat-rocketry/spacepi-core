#include <stdio.h>
#include <time.h>
#include <spacepi.h>
#include "encoder.h"
#include "led.h"
#include "timing.h"

int encode_bit(int bit, struct timespec *timer);

int encode(uint32_t ipaddr, struct timespec *timer) {
    for (uint32_t mask = (1 << 31); mask; mask >>= 1) {
        CHECK_ERROR(encode_bit, ipaddr & mask, timer);
    }
    return 0;
}

int encode_bit(int bit, struct timespec *timer) {
    struct timespec high_timer = *timer;
    CHECK_ERROR(set_led, 1);
    if (bit) {
        CHECK_ERROR(sleep_until, &high_timer, encoding_one_high);
    } else {
        CHECK_ERROR(sleep_until, &high_timer, encoding_zero_high);
    }
    CHECK_ERROR(set_led, 0);
    CHECK_ERROR(sleep_until, timer, encoding_period);
    return 0;
}
