#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <time.h>

int encode(uint32_t ipaddr, struct timespec *timer);

#endif
