#include <stdio.h>
#include <time.h>
#include <spacepi.h>
#include "encoder.h"
#include "led.h"

int encode(uint32_t ipaddr, struct timespec *timer) {
    printf("%d.%d.%d.%d\n", (ipaddr >> 24) & 0xFF, (ipaddr >> 16) & 0xFF, (ipaddr >> 8) & 0xFF, (ipaddr >> 0) & 0xFF);
    return 0;
}
