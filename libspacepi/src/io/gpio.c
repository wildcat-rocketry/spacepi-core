#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <spacepi-private.h>

#define NUM_PINS 28
#define DEVICE_TREE_NAME_MAX 200

static int open_device_tree(const char *alias, const char *field);
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

typedef struct {
    void (*callback)(void *context);
    void *context;
    int fd;
} irq_t;

typedef struct __attribute__((packed)) {
    uint32_t GPFSEL[6]; uint32_t __resv_0;
    uint32_t GPSET[2]; uint32_t __resv_1;
    uint32_t GPCLR[2]; uint32_t __resv_2;
    uint32_t GPLEV[2]; uint32_t __resv_3;
    uint32_t GPEDS[2]; uint32_t __resv_4;
    uint32_t GPREN[2]; uint32_t __resv_5;
    uint32_t GPFEN[2]; uint32_t __resv_6;
    uint32_t GPHEN[2]; uint32_t __resv_7;
    uint32_t GPLEN[2]; uint32_t __resv_8;
    uint32_t GPAREN[2]; uint32_t __resv_9;
    uint32_t GPAFEN[2]; uint32_t __resv_10;
    uint32_t GPPUD;
    uint32_t GPPUDCLK[2]; uint32_t __resv_11;
    uint32_t test;
} bcm2835_regs_t;

static volatile bcm2835_regs_t *bcm2835;
static volatile irq_t irqs[NUM_PINS];
static volatile int isr_running;
static pthread_mutex_t isr_mutex;

static int open_device_tree(const char *alias, const char *field) {
    char buf[DEVICE_TREE_NAME_MAX + 1];
    snprintf(buf, DEVICE_TREE_NAME_MAX, "/proc/device-tree/aliases/%s", alias);
    int fd = open(buf, O_RDONLY);
    if (fd < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    char *it = buf + sizeof("/proc/device-tree") - 1;
    int len = DEVICE_TREE_NAME_MAX - sizeof("/proc/device-tree");
    int r = read(fd, it, len);
    if (r < 0) {
        spacepi_perror("read", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    it += r - 1;
    len -= r;
    CHECK_ERROR(close, fd);
    strncpy(it, field, len);
    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        RETURN_REPORTED_ERROR();
    }
    return fd;
}

static int io_init_device(unsigned address, void **context) {
    static int needs_global_init = 1;
    if (needs_global_init) {
        int fd = open_device_tree("soc", "/ranges");
        if (fd < 0) {
            spacepi_perror("open_device_tree", __FILE__, __LINE__);
            RETURN_REPORTED_ERROR();
        }
        uint32_t ranges[2]; // range[0] is the physical offset, range[1] is the virtual offset
        CHECK_ERROR(read, fd, ranges, 8);
        ranges[0] = ntohl(ranges[0]);
        ranges[1] = ntohl(ranges[1]);
        CHECK_ERROR(close, fd);
        fd = open_device_tree("gpio", "/reg");
        if (fd < 0) {
            spacepi_perror("open_device_tree", __FILE__, __LINE__);
            RETURN_REPORTED_ERROR();
        }
        uint32_t reg[2]; // reg[0] is the physical offset, reg[1] is the length
        CHECK_ERROR(read, fd, reg, 8);
        reg[0] = ntohl(reg[0]);
        reg[1] = ntohl(reg[1]);
        CHECK_ERROR(close, fd);
        if (reg[1] < sizeof(bcm2835_regs_t)) {
            fflush(stdout);
            fprintf(stderr, "Register length is %u, but should be %u.\n", reg[1], sizeof(bcm2835_regs_t));
            RETURN_REPORTED_ERROR();
        }
        fd = open("/dev/mem", O_RDWR | O_SYNC);
        if (fd < 0) {
            spacepi_perror("open", __FILE__, __LINE__);
            RETURN_REPORTED_ERROR();
        }
        __off_t addr = reg[0] - ranges[0] + ranges[1];
        bcm2835 = (volatile bcm2835_regs_t *) mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);
        if (bcm2835 == MAP_FAILED) {
            spacepi_perror("mmap", __FILE__, __LINE__);
            CHECK_ERROR(close, fd);
            RETURN_REPORTED_ERROR();
        }
        CHECK_ERROR(close, fd);
        for (int i = 0; i < NUM_PINS; ++i) {
            irqs[i].fd = -1;
        }
        isr_running = 0;
        CHECK_ERROR(pthread_mutex_init, &isr_mutex, NULL);
        needs_global_init = 0;
    }
    if (address != 0) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    *context = NULL;
    return 0;
}

static int io_validate_pin(void *context, unsigned address, unsigned pinno) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    return 0;
}

static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    volatile uint32_t *reg = (volatile uint32_t *) &bcm2835->GPFSEL[pinno / 10];
    *reg &= ~(7 << ((pinno % 10) * 3));
    if (mode & sm_input_hi_z) {
        switch (mode) {
            case sm_input_hi_z:
                bcm2835->GPPUD = 0;
                break;
            case sm_input_pullup:
                bcm2835->GPPUD = 2;
                break;
            case sm_input_pulldown:
                bcm2835->GPPUD = 1;
                break;
        }
        struct timespec tm = {
            .tv_sec = 0,
            .tv_nsec = 600 // 150 clock cycles at 250 MHz
        };
        CHECK_ERROR(nanosleep, &tm, NULL);
        bcm2835->GPPUDCLK[0] = (1 << pinno);
        bcm2835->GPPUDCLK[1] = 0;
        CHECK_ERROR(nanosleep, &tm, NULL);
        bcm2835->GPPUD = 0;
        bcm2835->GPPUDCLK[0] = 0;
        bcm2835->GPPUDCLK[1] = 0;
    } else {
        *reg |= (1 << ((pinno % 10) * 3));
    }
    return 0;
}

static int io_write(void *context, unsigned address, unsigned pinno, int value) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (value) {
        volatile uint32_t *reg = (volatile uint32_t *) &bcm2835->GPSET[0];
        *reg = (1 << pinno);
    } else {
        volatile uint32_t *reg = (volatile uint32_t *) &bcm2835->GPCLR[0];
        *reg = (1 << pinno);
    }
    return 0;
}

static int io_read(void *context, unsigned address, unsigned pinno) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (bcm2835->GPLEV[0] & (1 << pinno)) {
        return HIGH;
    } else {
        return LOW;
    }
}

static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    CHECK_ERROR(pthread_mutex_lock, &isr_mutex);
    int err = 1;
    irqs[pinno].callback = callback;
    irqs[pinno].context = callback_context;
    switch (edge) {
        case si_none:
            bcm2835->GPREN[0] &= ~(1 << pinno);
            bcm2835->GPFEN[0] &= ~(1 << pinno);
            if (irqs[pinno].fd >= 0) {
                int fd = irqs[pinno].fd;
                irqs[pinno].fd = -1;
                CHECK_ERROR_JUMP(unlock_mutex, close, fd);
            }
            goto end;
        case si_rising:
            bcm2835->GPREN[0] |= (1 << pinno);
            bcm2835->GPLEN[0] &= ~(1 << pinno);
            break;
        case si_falling:
            bcm2835->GPREN[0] &= ~(1 << pinno);
            bcm2835->GPLEN[0] |= (1 << pinno);
            break;
        case si_both:
            bcm2835->GPREN[0] |= (1 << pinno);
            bcm2835->GPLEN[0] |= (1 << pinno);
            break;
    }
    if (irqs[pinno].fd < 0) {
        int fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd < 0) {
            spacepi_perror("open", __FILE__, __LINE__);
            goto unlock_mutex;
        }
        char buf[sizeof("/sys/class/gpio/gpioXX/value")];
        int len = sprintf(buf, "%d", pinno);
        CHECK_ERROR_JUMP(unlock_mutex, write, fd, buf, len);
        CHECK_ERROR_JUMP(unlock_mutex, close, fd);
        sprintf(buf, "/sys/class/gpio/gpio%d/value", pinno);
        irqs[pinno].fd = open(buf, O_RDONLY);
        if (irqs[pinno].fd < 0) {
            spacepi_perror("open", __FILE__, __LINE__);
            goto unlock_mutex;
        }
        if (!isr_running) {
            pthread_t thread;
            CHECK_ERROR_JUMP(unlock_mutex, pthread_create, &thread, NULL, isr_thread, NULL);
            isr_running = 1;
        }
    }
    end:
    err = 0;
    unlock_mutex:
    CHECK_ERROR(pthread_mutex_unlock, &isr_mutex);
    if (err) {
        RETURN_REPORTED_ERROR();
    } else {
        return 0;
    }
}

static void *isr_thread(void *context) {
    struct pollfd polls[NUM_PINS];
    int _pinno[NUM_PINS];
    char buf[sizeof("/sys/class/gpio/gpioXX/value")];
    while (1) {
        struct pollfd *it = polls;
        int *pinno = _pinno;
        if (pthread_mutex_lock(&isr_mutex) < 0) {
            spacepi_perror("pthread_mutex_lock", __FILE__, __LINE__);
            continue;
        }
        for (int i = 0; i < NUM_PINS; ++i) {
            if (irqs[i].fd >= 0) {
                it->fd = irqs[i].fd;
                it->events = POLLPRI;
                *pinno++ = i;
                ++it;
            }
        }
        if (pthread_mutex_unlock(&isr_mutex) < 0) {
            spacepi_perror("pthread_mutex_unlock", __FILE__, __LINE__);
        }
        if (it == polls) {
            isr_running = 0;
            return NULL;
        }
        if (poll(polls, it - polls, -1) < 0) {
            spacepi_perror("poll", __FILE__, __LINE__);
            continue;
        }
        if (pthread_mutex_lock(&isr_mutex) < 0) {
            spacepi_perror("pthread_mutex_lock", __FILE__, __LINE__);
        }
        for (--it; it >= polls; --it) {
            --pinno;
            if (it->revents & POLLPRI) {
                irqs[*pinno].callback(irqs[*pinno].context);
            } else if (it->revents != 0) {
                fprintf(stderr, "Error while waiting for interrupt on pin %d.\n", *pinno);
                sprintf(buf, "/sys/class/gpio/gpio%d/value", *pinno);
                irqs[*pinno].fd = open(buf, O_RDONLY);
                if (irqs[*pinno].fd < 0) {
                    spacepi_perror("open", __FILE__, __LINE__);
                }
            }
        }
        if (pthread_mutex_unlock(&isr_mutex) < 0) {
            spacepi_perror("pthread_mutex_unlock", __FILE__, __LINE__);
        }
    }
}
