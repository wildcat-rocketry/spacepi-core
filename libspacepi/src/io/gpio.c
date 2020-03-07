#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <linux/gpio.h>
#include <linux/limits.h>
#include <poll.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <spacepi-private.h>
#define CONSUMER "libspacepi"

#define BLOCK_DEVICE "/dev/gpiochip%d"

typedef struct {
    int fd;
    uint8_t def;
    int irq_fd;
    void (*callback)(void *context);
    void *callback_context;
} gpio_pin_t;

typedef struct {
    int fd;
    gpio_pin_t *pins;
    int num_pins;
    char consumer_name[32];
    int irq_pipe[2];
} gpio_t;

static int io_init_device(unsigned address, void **context);
static int io_validate_pin(void *context, unsigned address, unsigned pinno);
static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode);
static int io_write(void *context, unsigned address, unsigned pinno, int value);
static int io_read(void *context, unsigned address, unsigned pinno);
static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context);
static void *isr_thread(void *context);

DEFINE_THREAD_ENQUEUE_1(call_isr, void *);

spacepi_io_driver_t io_driver_gpio = {
    .name = "gpio",
    .init_device = io_init_device,
    .validate_pin = io_validate_pin,
    .mode = io_mode,
    .write = io_write,
    .read = io_read,
    .attach_isr = io_attach_isr,
    .instances = NULL
};

static int io_init_device(unsigned address, void **context) {
    int err = TRUE;
    char path[PATH_MAX];
    CHECK_ERROR(readlink, "/proc/self/exe", path, PATH_MAX);
    CHECK_ALLOC_DEF(gpio, gpio_t);
    char *last_slash = strrchr(path, '/');
    if (last_slash) {
        strncpy(gpio->consumer_name, last_slash + 1, sizeof(gpio->consumer_name) - 1);
    }
    char dev_name[sizeof(BLOCK_DEVICE) + 8];
    snprintf(dev_name, sizeof(dev_name) - 1, BLOCK_DEVICE, address);
    gpio->fd = open(dev_name, 0);
    if (gpio->fd < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        goto free_gpio;
    }
    struct gpiochip_info cinfo;
    CHECK_ERROR_JUMP(close_fd, ioctl, gpio->fd, GPIO_GET_CHIPINFO_IOCTL, &cinfo);
    gpio->num_pins = cinfo.lines;
    CHECK_ALLOC_ARRAY_JUMP(close_fd, gpio->pins, gpio_pin_t, gpio->num_pins);
    memset(gpio->pins, 0, sizeof(gpio_pin_t) * gpio->num_pins);
    for (int i = 0; i < gpio->num_pins; ++i) {
        gpio->pins[i].fd = -1;
        gpio->pins[i].irq_fd = -1;
    }
    CHECK_ERROR_JUMP(free_mem, pipe, gpio->irq_pipe);
    pthread_t thread;
    CHECK_ERROR_JUMP(free_pipe, pthread_create, &thread, NULL, isr_thread, gpio);
    
    *context = gpio;
    err = FALSE;
    if (FALSE) {
        free_pipe:
        close(gpio->irq_pipe[0]);
        close(gpio->irq_pipe[1]);
        free_mem:
        free(gpio->pins);
        close_fd:
        close(gpio->fd);
        free_gpio:
        free(gpio);
    }
    if (err) {
        RETURN_REPORTED_ERROR();
    }
    return 0;
}

static int io_validate_pin(void *context, unsigned address, unsigned pinno) {
    gpio_t *gpio = (gpio_t *) context;
    if (pinno >= gpio->num_pins) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    return 0;
}

static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode) {
    CHECK_ERROR(io_validate_pin, context, address, pinno);
    gpio_t *gpio = (gpio_t *) context;
    gpio_pin_t *pin = gpio->pins + pinno;
    if (pin->fd >= 0) {
        CHECK_ERROR_JUMP(reset_fd, close, pin->fd);
        reset_fd:
        pin->fd = -1;
    }
    struct gpiohandle_request req = {
        .lineoffsets = { pinno },
        .default_values = { pin->def },
        .lines = 1
    };
    if (mode & sm_input_hi_z) {
        req.flags = GPIOHANDLE_REQUEST_INPUT;
#ifdef GPIOLINE_FLAG_BIAS_PULL_DOWN
        if (mode & sm_input_pulldown) {
            req.flags |= GPIOLINE_FLAG_BIAS_PULL_DOWN;
        }
#endif
#ifdef GPIOLINE_FLAG_BIAS_PULL_UP
        if (mode & sm_input_pullup) {
            req.flags |= GPIOLINE_FLAG_BIAS_PULL_UP;
        }
#endif
    } else {
        req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    }
    strcpy(req.consumer_label, gpio->consumer_name);
    CHECK_ERROR(ioctl, gpio->fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    pin->fd = req.fd;
    return 0;
}

static int io_write(void *context, unsigned address, unsigned pinno, int value) {
    CHECK_ERROR(io_validate_pin, context, address, pinno);
    gpio_t *gpio = (gpio_t *) context;
    gpio_pin_t *pin = gpio->pins + pinno;
    pin->def = value;
    if (pin->fd < 0) {
        return 0;
    }
    struct gpiohandle_data data = {
        .values = { value }
    };
    CHECK_ERROR(ioctl, pin->fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
    return 0;
}

static int io_read(void *context, unsigned address, unsigned pinno) {
    CHECK_ERROR(io_validate_pin, context, address, pinno);
    gpio_t *gpio = (gpio_t *) context;
    gpio_pin_t *pin = gpio->pins + pinno;
    if (pin->fd < 0) {
        return pin->def;
    }
    struct gpiohandle_data data;
    CHECK_ERROR(ioctl, pin->fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
    return data.values[0];
}

static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context) {
    CHECK_ERROR(io_validate_pin, context, address, pinno);
    gpio_t *gpio = (gpio_t *) context;
    gpio_pin_t *pin = gpio->pins + pinno;
    char buf;
    if (pin->irq_fd >= 0) {
        CHECK_ERROR_JUMP(reset_fd, close, pin->irq_fd);
        reset_fd:
        1;
        int fd = pin->irq_fd;
        pin->irq_fd = -1;
        CHECK_ERROR_JUMP(after_reset_fd, write, gpio->irq_pipe[1], &buf, 1);
        after_reset_fd:
        close(fd);
    }
    struct gpioevent_request req = {
        .lineoffset = pinno,
        .handleflags = 0,
        .eventflags = 0
    };
    if (edge & si_rising) {
        req.eventflags |= GPIOEVENT_REQUEST_RISING_EDGE;
    }
    if (edge & si_falling) {
        req.eventflags |= GPIOEVENT_REQUEST_FALLING_EDGE;
    }
    CHECK_ERROR(ioctl, pin->fd, GPIO_GET_LINEEVENT_IOCTL, &req);
    pin->irq_fd = pin->fd;
    CHECK_ERROR(write, gpio->irq_pipe[1], &buf, 1);
    return 0;
}

static void *isr_thread(void *context) {
    gpio_t *gpio = (gpio_t *) context;
    CHECK_ALLOC_ARRAY_DEF_JUMP(alloc_err, fds, struct pollfd, gpio->num_pins + 1);
    CHECK_ALLOC_ARRAY_DEF_JUMP(alloc_err_map, map, gpio_pin_t *, gpio->num_pins);
    if (FALSE) {
        alloc_err_map:
        free(fds);
        alloc_err:
        fprintf(stderr, "isr_thread: Unable to allocate %d file descriptors for poll()\n", gpio->num_pins + 1);
        return NULL;
    }
    fds[0].fd = gpio->irq_pipe[0];
    for (int i = 0; i <= gpio->num_pins; ++i) {
        fds[i].events = POLLIN;
    }
    struct gpioevent_data ev;
    while (TRUE) {
        struct pollfd *it = fds + 1;
        gpio_pin_t **mit = map;
        for (int i = 0; i < gpio->num_pins; ++i) {
            gpio_pin_t *pin = gpio->pins + i;
            if (pin->irq_fd >= 0) {
                it++->fd = pin->irq_fd;
                *mit++ = pin;
            }
        }
        CHECK_ERROR_JUMP(retry, poll, fds, it - fds, -1);
        if (fds->revents & POLLIN) {
            char buf;
            CHECK_ERROR_JUMP(ignore1, read, fds->fd, &buf, 1);
            ignore1:
            1;
        }
        for (int i = 0; i < gpio->num_pins; ++i) {
            if (fds[i+1].revents & POLLIN) {
                CHECK_ERROR_JUMP(ignore2, read, fds[i+1].fd, &ev, sizeof(struct gpioevent_data));
                CHECK_ERROR_JUMP(ignore2, call_isr, map[i]->callback, map[i]->callback_context);
                if (FALSE) {
                    ignore2:
                    continue;
                }
            }
        }

        retry:
        1;
    }
    return NULL;
}
