#include <linux/i2c-dev.h>
#include <spacepi-private.h>

#define FILENAME_ROOT "/dev/i2c-"
#define DEVICE_ID_ADDR 0b1111100

typedef enum {
    NXP = 0
} manufacturer_t;

typedef enum {
    PCA9571
} part_id_t;

typedef struct {
    unsigned revision:3;
    part_id_t part_id:9;
    manufacturer_t manufacturer:12;
} device_id_t;

static const char *manufacturer_string(manufacturer_t manufacturer);
static const char *part_id_string(part_id_t part_id);
static int io_init_device(unsigned address, void **context);
static int io_validate_pin(void *context, unsigned address, unsigned pinno);
static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode);
static int io_write(void *context, unsigned address, unsigned pinno, int value);
static int io_read(void *context, unsigned address, unsigned pinno);
static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context);

spacepi_io_driver_t io_driver_gpio = {
    .name = "PCA9571",
    .init_device = io_init_device,
    .validate_pin = io_validate_pin,
    .mode = io_mode,
    .write = io_write,
    .read = io_read,
    .attach_isr = io_attach_isr,
    .instances = NULL
};

static const char *manufacturer_string(manufacturer_t manufacturer) {
    switch (manufacturer) {
        case NXP:
            return "NXP";
        default:
            return "Unknown manufacturer";
    }
}

static const char *part_id_string(part_id_t part_id) {
    switch (part_id) {
        case PCA9571:
            return "PCA9571";
        default:
            return "Unknown part";
    }
}

static int io_init_device(unsigned address, void **context) {
    unsigned i2cdev_id = address >> 7;
    address &= 0x7F;
    CHECK_ALLOC_ARRAY_DEF(filename, char, sizeof(FILENAME_ROOT) + (i2cdev_id >> 3) + 1);
    sprintf(filename, FILENAME_ROOT "%d", i2cdev_id);
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        spacepi_perror("open", __FILE__, __LINE__);
        free(filename);
        RETURN_REPORTED_ERROR();
    }
    free(filename);
    CHECK_ERROR_JUMP(close_fd, ioctl, I2C_SLAVE, DEVICE_ID_ADDR);
    unsigned char buf[3] = { (unsigned char) address };
    struct i2c_msg msgs[] = {
        {
            .addr = address,
            .flags = 0,
            .len = 1,
            .buf = buf
        },
        {
            .addr = address,
            .flags = I2C_M_RD,
            .len = 3,
            .buf = buf
        }
    };
    struct i2c_rdwr_ioctl_data msgset = {
        .msgs = msgs,
        .nmsgs = 2
    };
    CHECK_ERROR_JUMP(close_fd, ioctl, I2C_RDWR, &msgset);
    device_id_t *dev_id = (device_id_t *) buf;
    printf("Connected to %s by %s (rev %d)\n", part_id_string(dev_id->part_id), manufacturer_string(dev_id->manufacturer), dev_id->revision);
    *context = (void *) fd;
    return 0;
    close_fd:
    close(fd);
    RETURN_REPORTED_ERROR();
}

static int io_validate_pin(void *context, unsigned address, unsigned pinno) {
    if (pinno >= 8) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    return 0;
}

static int io_mode(void *context, unsigned address, unsigned pinno, pin_mode_t mode) {
    if (mode != output) {
        RETURN_ERROR_SYSTEM(ENOTSUP);
    }
    return 0;
}

static int io_write(void *context, unsigned address, unsigned pinno, int value) {
    if (pinno >= 8) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    int fd = (int) context;
    unsigned char port;
    CHECK_ERROR(read, fd, &port, 1);
    if (value) {
        port |= (1 << pinno);
    } else {
        port &= ~(1 << pinno);
    }
    CHECK_ERROR(write, fd, &port, 1);
    return 0;
}

static int io_read(void *context, unsigned address, unsigned pinno) {
    if (pinno >= 8) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    int fd = (int) context;
    unsigned char port;
    CHECK_ERROR(read, fd, &port, 1);
    return (port >> pinno) & 1;
}

static int io_attach_isr(void *context, unsigned address, unsigned pinno, edge_t edge, void (*callback)(void *context), void *callback_context) {
    RETURN_ERROR_SYSTEM(ENOTSUP);
}
