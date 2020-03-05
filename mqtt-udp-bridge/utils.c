#include "utils.h"
#include <stdlib.h>
#include <string.h>

void buf_init(buf_t * buf, int capacity) {
    if (buf != NULL) {
        buf->buf = malloc(capacity * sizeof(int));
        memset(buf->buf, 0, capacity * sizeof(int));
        buf->length = capacity;
    }
}

void buf_add(buf_t * buf, int value) {
    if (buf != NULL) {
        int i;
        for (i = 0; i < buf->length; i++) {
            if (buf->buf[i] == 0) {
                buf->buf[i] = value;
                break;
            }
        }
        // at this point we cannot fit it into the array
    }
}

void buf_remove(buf_t * buf, int value) {
    if (buf != NULL) {
        int i;
        for (i = 0; i < buf->length; i++) {
            if (buf->buf[i] == value) {
                buf->buf[i] = 0;
                break;
            }
        }
    }
}

int hash_buf(char * buf, size_t size) { // probably a terrible hash function
    int prime = 31;
    int hash = 0;
    int i;
    for (i = 0; i < size; i++) {
        hash = (hash + buf[i]) * prime;
    }
    return hash;
}