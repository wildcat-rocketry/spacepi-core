#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <spacepi-private.h>

const char const base_62[63] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

int random_bytes(void *buffer, size_t count) {
    static int fd = -1;
    if (fd < 0) {
        fd = open("/dev/urandom", O_RDONLY);
        if (fd < 0) {
            spacepi_perror("open", __FILE__, __LINE__);
            RETURN_REPORTED_ERROR();
        }
    }
    while (count > 0) {
        ssize_t r = read(fd, buffer, count);
        if (r < 0) {
            spacepi_perror("read", __FILE__, __LINE__);
            close(fd);
            fd = -1;
            RETURN_REPORTED_ERROR();
        } else if (r == 0) {
            RETURN_ERROR_SPACEPI(NO_RANDOM_DATA);
        } else {
            count -= r;
        }
    }
    return 0;
}

int random_byte(void) {
    char c;
    int r = random_bytes(&c, 1);
    if (r < 0) {
        return r;
    } else {
        return c;
    }
}

int randomize_string(const void *alphabet, size_t word_size, size_t alphabet_size, void *buffer, size_t num_words) {
    for (; num_words > 0; --num_words) {
        int rc;
        for (int c = 1; alphabet_size > c; c <<= 8) {
            int r = random_byte();
            if (r < 0) {
                spacepi_perror("random_byte", __FILE__, __LINE__);
                RETURN_REPORTED_ERROR();
            }
            rc = ((rc << 8) | r);
        }
        memcpy(buffer, alphabet + word_size * (rc % alphabet_size), word_size);
        buffer += word_size;
    }
    return 0;
}
