#include <stdlib.h>
#include <string.h>
#include <spacepi.h>

const char const base_62[63] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

int random_reseed(void) {
    time_t t;
    time(&t);
    srand((unsigned int) t);
    return 0;
}

int randomize_string(const void *alphabet, size_t word_size, size_t alphabet_size, void *buffer, size_t num_words) {
    for (; num_words > 0; --num_words) {
        memcpy(buffer, alphabet + word_size * (rand() % alphabet_size), word_size);
        buffer += word_size;
    }
    return 0;
}
