#include <stdio.h>
#include <spacepi.h>
#include "led.h"

int led_init(void) {
    return 0;
}

int set_led(int val) {
    printf("%d\n", val);
    return 0;
}
