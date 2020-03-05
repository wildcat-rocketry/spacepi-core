#ifndef __utils_h__
#define __utils_h__

#include <stdlib.h>

typedef struct buf {
    int * buf;
    int length;
} buf_t;

void buf_init(buf_t *, int);
void buf_add(buf_t *, int);
void buf_remove(buf_t *, int);

int hash_buf(char *, size_t);

#endif