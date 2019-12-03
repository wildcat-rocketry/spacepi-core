#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <spacepi-private.h>

//#define NC_DEBUG

static int io_init_device(unsigned address, void **context);
static int io_validate_pin(void *context, unsigned address, unsigned pinno);
static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode);
static int io_write(void *context, unsigned address, unsigned pinno, int value);
static int io_read(void *context, unsigned address, unsigned pinno);
static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context);

spacepi_io_driver_t io_driver_nc = {
    .name = "nc",
    .init_device = io_init_device,
    .validate_pin = io_validate_pin,
    .mode = io_mode,
    .write = io_write,
    .read = io_read,
    .attach_isr = io_attach_isr,
    .instances = NULL
};

static int io_init_device(unsigned address, void **context) {
    return 0;
}

static int io_validate_pin(void *context, unsigned address, unsigned pinno) {
    return 0;
}

static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode) {
    if (mode != output) {
        RETURN_ERROR_SYSTEM(ENOTSUP);
    }
    return 0;
}

static int io_write(void *context, unsigned address, unsigned pinno, int value) {
#ifdef NC_DEBUG
    printf("nc: pin %d set to %d\n", pinno, value);
#endif
    return 0;
}

static int io_read(void *context, unsigned address, unsigned pinno) {
    RETURN_ERROR_SYSTEM(ENOTSUP);
}

static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context) {
    RETURN_ERROR_SYSTEM(ENOTSUP);
}
