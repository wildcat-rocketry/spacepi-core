#include <sys/types.h>
#include <unistd.h>
#include <wiringPi.h>
#include <spacepi-private.h>

#define NUM_PINS 28

static int io_init_device(unsigned address, void **context);
static int io_validate_pin(void *context, unsigned address, unsigned pinno);
static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode);
static int io_write(void *context, unsigned address, unsigned pinno, int value);
static int io_read(void *context, unsigned address, unsigned pinno);
static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context);
static void isr(void);

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
    edge_t edge;
    unsigned char last_val;
} irq_t;

static irq_t irqs[NUM_PINS];

static int io_init_device(unsigned address, void **context) {
    static int needs_global_init = 1;
    if (needs_global_init) {
        if (geteuid() == 0) {
            wiringPiSetupGpio();
        } else {
            wiringPiSetupSys();
        }
        for (int i = 0; i < NUM_PINS; ++i) {
            irqs[i].edge = none;
        }
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
    if (mode & input_hi_z) {
        pinMode(pinno, INPUT);
        switch (mode) {
            case input_hi_z:
                pullUpDnControl(pinno, PUD_OFF);
                break;
            case input_pullup:
                pullUpDnControl(pinno, PUD_UP);
                break;
            case input_pulldown:
                pullUpDnControl(pinno, PUD_DOWN);
                break;
        }
    } else {
        pinMode(pinno, OUTPUT);
    }
    return 0;
}

static int io_write(void *context, unsigned address, unsigned pinno, int value) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    digitalWrite(pinno, value);
}

static int io_read(void *context, unsigned address, unsigned pinno) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    return digitalRead(pinno);
}

static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context) {
    if (pinno >= NUM_PINS) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    irqs[pinno].callback = callback;
    irqs[pinno].context = callback_context;
    if (irqs[pinno].edge != edge) {
        wiringPiISR(pinno, edge, isr);
    }
    irqs[pinno].edge = edge;
    int e = io_read(context, address, pinno);
    if (e < 0) {
        return e;
    }
    irqs[pinno].last_val = (unsigned char) e;
    return 0;
}

static void isr(void) {
    for (unsigned i = 0; i < NUM_PINS; ++i) {
        if (irqs[i].edge != none) {
            int e = io_read(NULL, 0, i);
            if (e >= 0 && e != irqs[i].last_val) {
                irqs[i].last_val = (unsigned char) e;
                if ((e && (irqs[i].edge & rising)) || (!e && (irqs[i].edge & falling))) {
                    call_isr(irqs[i].callback, irqs[i].context);
                }
            }
        }
    }
}
