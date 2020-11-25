#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/SPI.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockSPI : public SPI, private spacepi::log::AutoLog<decltype("core:resource:mock:spi"_autolog)> {
                public:
                    MockSPI(const std::string &name) noexcept;

                    enum Mode getMode() const noexcept;
                    void setMode(enum Mode mode);

                    int getSpeed() const noexcept;
                    void setSpeed(int speed);

                protected:
                    void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps);

                private:
                    std::string name;
                    enum Mode mode;
                    int speed;
            };

            class MockSPIFactory : public ResourceFactory<SPI> {
                public:
                    static MockSPIFactory instance;

                    MockSPIFactory();

                private:
                    std::shared_ptr<SPI> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockSPIFactory MockSPIFactory::instance;

MockSPI::MockSPI(const string &name) noexcept : name(name), mode(SPI::Mode0), speed(100000) {
    log(LogLevel::Debug) << "Created new SPI '" << name << "'.";
}

enum SPI::Mode MockSPI::getMode() const noexcept {
    return mode;
}

void MockSPI::setMode(enum SPI::Mode mode) {
    log(LogLevel::Debug) << "Setting SPI '" << name << "' to " << mode << ".";
    this->mode = mode;
}

int MockSPI::getSpeed() const noexcept {
    return speed;
}

void MockSPI::setSpeed(int speed) {
    log(LogLevel::Debug) << "Setting SPI '" << name << "' frequency to " << speed << ".";
    this->speed = speed;
}

void MockSPI::doTransaction(const vector<pair<uint8_t *, int16_t>> &steps) {
    ostream &os = log(LogLevel::Debug) << "SPI transaction:";
    for (vector<pair<uint8_t *, int16_t>>::const_iterator it = steps.begin(); it != steps.end(); ++it) {
        if (it->second < 0) {
            os << "\n    Write of " << -it->second << " bytes:" << hex << setw(2);
            for (int i = 0; i < -it->second; ++i) {
                os << " 0x" << it->first[i];
            }
            os << setw(0) << dec << ".";
        } else {
            os << "\n    Read of " << it->second << " bytes.";
        }
    }
}

MockSPIFactory::MockSPIFactory() : ResourceFactory<SPI>("mock") {
}

shared_ptr<SPI> MockSPIFactory::create(const string &name) {
    return shared_ptr<SPI>(new MockSPI(name));
}

shared_ptr<SPI> SPI::get(const string &name) {
    return ResourceFactory<SPI>::get(name);
}

ostream &operator <<(ostream &os, enum SPI::Mode mode) {
    switch (mode) {
        case SPI::Mode0 | SPI::MSBFirst:
            return os << "Mode 0 (Falling Edge, Active High), MSB First";
        case SPI::Mode1 | SPI::MSBFirst:
            return os << "Mode 1 (Rising Edge, Active High), MSB First";
        case SPI::Mode2 | SPI::MSBFirst:
            return os << "Mode 2 (Falling Edge, Active Low), MSB First";
        case SPI::Mode3 | SPI::MSBFirst:
            return os << "Mode 3 (Rising Edge, Active Low), MSB First";
        case SPI::Mode0 | SPI::LSBFirst:
            return os << "Mode 0 (Falling Edge, Active High), LSB First";
        case SPI::Mode1 | SPI::LSBFirst:
            return os << "Mode 1 (Rising Edge, Active High), LSB First";
        case SPI::Mode2 | SPI::LSBFirst:
            return os << "Mode 2 (Falling Edge, Active Low), LSB First";
        case SPI::Mode3 | SPI::LSBFirst:
            return os << "Mode 3 (Rising Edge, Active Low), LSB First";
        default:
            return os << "Unknown mode " << (int) mode;
    }
}
