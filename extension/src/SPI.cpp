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
    throwError("opening device", fd);
    uint32_t m;
    throwError("setting mode", ioctl(fd, SPI_IOC_RD_MODE32, &m));
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
    throwError("setting speed", ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &s));
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
    throwError("setting mode", ioctl(fd, SPI_IOC_WR_MODE32, &m));
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
    throwError("setting speed", ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &s));
    this->speed = speed;
}

void SPI::doTransaction(const vector<pair<uint8_t *, int16_t>> &steps) {
    vector<uint8_t> mosi;
    int misoCount;
    for (vector<pair<uint8_t *, int16_t>>::const_iterator it = steps.begin(); it != steps.end(); ++it) {
        if (it->second < 0) {
            mosi.reserve(mosi.size() - it->second);
            for (int i = 0; i < -it->second; ++i) {
                mosi.push_back(it->first[i]);
            }
        } else {
            misoCount += it->second;
        }
    }
    if (mosi.size() > misoCount) {
        misoCount = mosi.size();
    } else if (mosi.size() < misoCount) {
        mosi.resize(misoCount);
    }
    vector<uint8_t> miso(misoCount);
    struct spi_ioc_transfer msg;
    memset(&msg, 0, sizeof(msg));
    msg.tx_buf = (uint64_t) mosi.data();
    msg.rx_buf = (uint64_t) miso.data();
    msg.len = misoCount;
    throwError("executing SPI transaction", ioctl(fd, SPI_IOC_MESSAGE(1), &msg));
    uint8_t *src = miso.data();
    for (vector<pair<uint8_t *, int16_t>>::const_iterator it = steps.begin(); it != steps.end(); ++it) {
        if (it->second > 0) {
            memcpy(it->first, src, it->second);
            src += it->second;
        }
    }
}

void SPI::throwError(const string &action, int returnCode) {
    if (returnCode < 0) {
        throw EXCEPTION(ResourceException("SPI failed " + action + ": " + strerror(errno)));
    }
}
