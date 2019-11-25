#include <spacepi.h>

int thread_pool_init(void) {
    // TODO
    return 0;
}

int thread_enqueue(void (*trampoline)(void *context), void *context) {
    // TODO
    trampoline(context);
    return 0;
}
