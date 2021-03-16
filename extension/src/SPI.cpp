#include <cstdint>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SpacePi.hpp>
#include <spacepi/target/extension/SPI.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::util;
using namespace spacepi::target::extension;

SPI::SPI(const string &deviceName) : fd(open(("/dev/" + deviceName).c_str(), O_RDWR)) {
    throwError(fd);
    uint32_t m;
    throwError(ioctl(fd, SPI_IOC_RD_MODE32, &m));
    switch (m & (SPI_MODE_0 | SPI_MODE_1 | SPI_MODE_2 | SPI_MODE_3)) {
        case SPI_MODE_0:
            mode = Mode0;
            break;
        case SPI_MODE_1:
            mode = Mode1;
            break;
        case SPI_MODE_2:
            mode = Mode2;
            break;
        case SPI_MODE_3:
            mode = Mode3;
            break;
        default:
            throw EXCEPTION(ResourceException("Unknown SPI mode reported by device"));
            break;
    }
    uint32_t s;
    throwError(ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &s));
    speed = s;
}

enum resource::SPI::Mode SPI::getMode() const noexcept {
    return mode;
}

void SPI::setMode(enum resource::SPI::Mode mode) {
    uint32_t m;
    switch (mode) {
        case Mode0:
            m = SPI_MODE_0;
            break;
        case Mode1:
            m = SPI_MODE_1;
            break;
        case Mode2:
            m = SPI_MODE_2;
            break;
        case Mode3:
            m = SPI_MODE_3;
            break;
        default:
            throw EXCEPTION(ArgumentException("Unknown SPI mode"));
            break;
    }
    throwError(ioctl(fd, SPI_IOC_WR_MODE32, &m));
    this->mode = mode;
}

int SPI::getSpeed() const noexcept {
    return speed;
}

void SPI::setSpeed(int speed) {
    if (speed < 0) {
        throw EXCEPTION(ArgumentException("Cannot have a negative frequency"));
    }
    uint32_t s = speed;
    throwError(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &s));
    this->speed = speed;
}

void SPI::doTransaction(const vector<pair<uint8_t *, int16_t>> &steps) {
    vector<struct spi_ioc_transfer> messages;
    for (vector<pair<uint8_t *, int16_t>>::const_iterator it = steps.begin(); it != steps.end(); ++it) {
        if (it->second < 0) {
            struct spi_ioc_transfer tx;
            memset(&tx, 0, sizeof(tx));
            tx.tx_buf = (uint64_t) it->first;
            tx.len = -it->second;
            messages.push_back(tx);
        }
    }
    vector<struct spi_ioc_transfer>::iterator mit = messages.begin();
    for (vector<pair<uint8_t *, int16_t>>::const_iterator sit = steps.begin(); sit != steps.end(); ++sit) {
        int16_t len = sit->second;
        uint64_t data = (uint64_t) sit->first;
        while (len > 0) {
            if (mit == messages.end()) {
                struct spi_ioc_transfer rx;
                memset(&rx, 0, sizeof(rx));
                rx.rx_buf = data;
                rx.len = len;
                messages.push_back(rx);
                len = 0;
            } else if (mit->len < len) {
                mit->rx_buf = data;
                data += mit->len;
                len -= mit->len;
            } else if (mit->len > len) {
                mit->rx_buf = data;
                struct spi_ioc_transfer tx;
                memset(&tx, 0, sizeof(tx));
                tx.tx_buf = mit->tx_buf + len;
                tx.len = mit->len - len;
                mit->len = len;
                mit = messages.insert(mit + 1, tx) - 1;
                len = 0;
            } else {
                mit++->rx_buf = data;
                len = 0;
            }
        }
    }
    int n = messages.size();
    throwError(ioctl(fd, SPI_IOC_MESSAGE(n), messages.data()));
}

void SPI::throwError(int returnCode) {
    if (returnCode < 0) {
        throw EXCEPTION(ResourceException("SPI operation failed. " + string(strerror(errno))));

    }
}
