#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <spacepi.h>
#include "backup.h"

int tty1;
int varfd;
uint32_t last_ip;

int backup_init(void) {
    tty1 = open("/dev/tty1", O_WRONLY | O_SYNC);
    if (tty1 < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    varfd = open("/var/local/ip", O_WRONLY | O_CREAT | O_SYNC);
    if (varfd < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    last_ip = 0;
    return 0;
}

int backup_write(uint32_t ipaddr) {
    if (last_ip != ipaddr) {
        char msg[sizeof("\r\033[KIP: \033[32m255.255.255.255\033[0m\n")];
        sprintf(msg, "\r\033[KIP: \033[32m%d.%d.%d.%d\033[0m\n", (ipaddr >> 24) & 0xFF, (ipaddr >> 16) & 0xFF, (ipaddr >> 8) & 0xFF, (ipaddr >> 0) & 0xFF);
        CHECK_ERROR(write, tty1, msg, strlen(msg));
        sprintf(msg, "%d.%d.%d.%d\n", (ipaddr >> 24) & 0xFF, (ipaddr >> 16) & 0xFF, (ipaddr >> 8) & 0xFF, (ipaddr >> 0) & 0xFF);
        int len = strlen(msg);
        CHECK_ERROR(lseek, varfd, 0, SEEK_SET);
        CHECK_ERROR(write, varfd, msg, len);
        CHECK_ERROR(ftruncate, varfd, len);
        CHECK_ERROR(syncfs, varfd);
    }
    return 0;
}
