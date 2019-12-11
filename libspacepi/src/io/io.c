#include <stdlib.h>
#include <string.h>
#define GENERATE_DRIVER_LIST
#include <spacepi-private.h>

int get_pin(const char *name, pin_t *pin) {
    const char *bus_end = name;
    int inverted = 0;
    if (*bus_end == '~' || *bus_end == '!') {
        inverted = 1;
        ++bus_end;
        ++name;
    }
    for (; *bus_end && *bus_end != '@' && *bus_end != ':'; ++bus_end);
    unsigned address = 0;
    unsigned pinno = 0;
    if (*bus_end) {
        const char *addr = bus_end + 1;
        const char *addr_end = addr;
        const char *pinnos;
        if (*bus_end == '@') {
            for (; *addr_end && *addr_end != ':'; ++addr_end) {
                address <<= 4;
                if (*addr_end >= '0' && *addr_end <= '9') {
                    address += *addr_end - '0';
                } else if (*addr_end >= 'A' && *addr_end <= 'F') {
                    address += *addr_end - 'A' + 10;
                } else if (*addr_end >= 'a' && *addr_end <= 'f') {
                    address += *addr_end - 'a' + 10;
                } else {
                    RETURN_ERROR_SPACEPI(INVALID_PIN);
                }
            }
            if (!*addr_end) {
                RETURN_ERROR_SPACEPI(INVALID_PIN);
            }
            pinnos = addr_end + 1;
        } else if (*bus_end == ':') {
            pinnos = bus_end + 1;
        } else {
            pinnos = bus_end;
        }
        for (const char *it = pinnos; *it; ++it) {
            if (*it >= '0' && *it <= '9') {
                pinno = (pinno * 10) + (*it - '0');
            } else {
                RETURN_ERROR_SPACEPI(INVALID_PIN);
            }
        }
    }
    spacepi_io_driver_t **driver;
    int n = bus_end - name;
    for (driver = io_drivers; *driver; ++driver) {
        if (strncmp((*driver)->name, name, n) == 0) {
            break;
        }
    }
    if (!*driver) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    pin->controller = (*driver)->name;
    pin->address = address;
    pin->pin = pinno;
    pin->inverted = inverted;
    spacepi_io_driver_instance_t *instance;
    for (instance = (*driver)->instances; instance; instance = instance->next) {
        if (instance->address == address) {
            CHECK_ERROR((*driver)->validate_pin, instance->context, address, pinno);
            pin->driver = instance;
            return 0;
        }
    }
    CHECK_ALLOC(instance, spacepi_io_driver_instance_t);
    CHECK_ERROR_JUMP(free_instance, (*driver)->init_device, address, &instance->context);
    instance->driver = *driver;
    instance->address = address;
    instance->next = (*driver)->instances;
    (*driver)->instances = instance;
    CHECK_ERROR((*driver)->validate_pin, instance->context, address, pinno);
    pin->driver = instance;
    return 0;

    free_instance:
    free(instance);
    RETURN_REPORTED_ERROR();
}

int pin_mode(pin_t *pin, pin_mode_t mode) {
    spacepi_io_driver_instance_t *driver = (spacepi_io_driver_instance_t *) pin->driver;
    if (!driver) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (pin->controller != driver->driver->name || pin->address != driver->address) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    return driver->driver->mode(driver->context, pin->address, pin->pin, mode);
}

int digital_write(pin_t *pin, int value) {
    spacepi_io_driver_instance_t *driver = (spacepi_io_driver_instance_t *) pin->driver;
    if (!driver) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (pin->controller != driver->driver->name || pin->address != driver->address) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (pin->inverted) {
        value = 1 - value;
    }
    return driver->driver->write(driver->context, pin->address, pin->pin, value);
}

int digital_read(pin_t *pin) {
    spacepi_io_driver_instance_t *driver = (spacepi_io_driver_instance_t *) pin->driver;
    if (!driver) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (pin->controller != driver->driver->name || pin->address != driver->address) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    int value = driver->driver->read(driver->context, pin->address, pin->pin);
    if (pin->inverted) {
        value = 1 - value;
    }
    return value;
}

int attach_interrupt(pin_t *pin, edge_t edge, void (*callback)(void *context), void *context) {
    spacepi_io_driver_instance_t *driver = (spacepi_io_driver_instance_t *) pin->driver;
    if (!driver) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (pin->controller != driver->driver->name || pin->address != driver->address) {
        RETURN_ERROR_SPACEPI(INVALID_PIN);
    }
    if (pin->inverted) {
        switch (edge) {
            case rising:
                edge = falling;
                break;
            case falling:
                edge = rising;
                break;
        }
    }
    return driver->driver->attach_isr(driver->context, pin->address, pin->pin, edge, callback, context);
}
