#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <spacepi.h>
#include "led.h"

#define LED_FILENAME "/sys/class/leds/led0"

int fd;

int led_init(void) {
    int trigger = open(LED_FILENAME "/trigger", O_WRONLY | O_SYNC);
    if (trigger < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    int err = 1;
    CHECK_ERROR_JUMP(cleanup_trigger, write, trigger, "none", 4);
    err = 0;
    cleanup_trigger:
    CHECK_ERROR(close, trigger);
    if (err) {
        RETURN_REPORTED_ERROR();
    }
    fd = open(LED_FILENAME "/brightness", O_WRONLY | O_SYNC);
    if (fd < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    return 0;
}

int set_led(int val) {
    CHECK_ERROR(lseek, fd, 0, SEEK_SET);
    if (val) {
        CHECK_ERROR(write, fd, "1", 1);
    } else {
        CHECK_ERROR(write, fd, "0", 1);
    }
    return 0;
}
