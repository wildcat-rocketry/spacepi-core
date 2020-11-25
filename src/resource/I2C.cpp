#include <cstdint>
#include <iomanip>
#include <ios>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/I2C.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockI2C : public I2C, private spacepi::log::AutoLog<decltype("core:resource:mock:i2c"_autolog)> {
                public:
                    MockI2C(const std::string &name) noexcept;

                    int getSpeed() const noexcept;
                    void setSpeed(int speed);

                    bool isSMBusErrorCheckingEnabled() const noexcept;
                    void enableSMBusErrorChecking();
                    void disableSMBusErrorChecking();

                    void quickWriteSMBus(bool val);
                    uint8_t readByteSMBus();
                    void writeByteSMBus(uint8_t data);
                    uint8_t readByteSMBus(uint8_t command);
                    void writeByteSMBus(uint8_t command, uint8_t data);
                    uint16_t readWordSMBus(uint8_t command);
                    void writeWordSMBus(uint8_t command, uint16_t data);
                    uint16_t processCallSMBus(uint8_t command, uint16_t data);
                    void readBlockSMBus(uint8_t command, uint8_t *data);
                    void writeBlockSMBus(uint8_t command, uint8_t *data, uint8_t length);

                protected:
                    void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps);

                private:
                    std::string name;
                    int speed;
                    bool errorChecking;
            };

            class MockI2CFactory : public ResourceFactory<I2C> {
                public:
                    static MockI2CFactory instance;

                    MockI2CFactory();

                private:
                    std::shared_ptr<I2C> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockI2CFactory MockI2CFactory::instance;

MockI2C::MockI2C(const string &name) noexcept : name(name), speed(100000), errorChecking(false) {
    log(LogLevel::Debug) << "Created new I2C '" << name << "'.";
}

int MockI2C::getSpeed() const noexcept {
    return speed;
}

void MockI2C::setSpeed(int speed) {
    log(LogLevel::Debug) << "Setting I2C '" << name << "' speed to " << speed << ".";
    this->speed = speed;
}

bool MockI2C::isSMBusErrorCheckingEnabled() const noexcept {
    return errorChecking;
}

void MockI2C::enableSMBusErrorChecking() {
    log(LogLevel::Debug) << "Enabling error checking for SMBus '" << name << "'.";
    errorChecking = true;
}

void MockI2C::disableSMBusErrorChecking() {
    log(LogLevel::Debug) << "Disabling error checking for SMBus '" << name << "'.";
    errorChecking = false;
}

void MockI2C::quickWriteSMBus(bool val) {
    log(LogLevel::Debug) << "Quick write to SMBus '" << name << "': " << val << ".";
}

uint8_t MockI2C::readByteSMBus() {
    log(LogLevel::Debug) << "Byte read from SMBus '" << name << "'.";
    return 0xFF;
}

void MockI2C::writeByteSMBus(uint8_t data) {
    log(LogLevel::Debug) << "Byte write to SMBus '" << name << "': 0x" << hex << setw(2) << data << setw(0) << ".";
}

uint8_t MockI2C::readByteSMBus(uint8_t command) {
    log(LogLevel::Debug) << "Byte read (command 0x" << hex << setw(2) << command << setw(0) << ") from SMBus '" << name << "'.";
    return 0xFF;
}

void MockI2C::writeByteSMBus(uint8_t command, uint8_t data) {
    log(LogLevel::Debug) << "Byte write (command 0x" << hex << setw(2) << command << setw(0) << ") to SMBus '" << name << "': 0x" << setw(2) << data << setw(0) << ".";
}

uint16_t MockI2C::readWordSMBus(uint8_t command) {
    log(LogLevel::Debug) << "Word read (command 0x" << hex << setw(2) << command << setw(0) << ") from SMBus '" << name << "'.";
    return 0xFFFF;
}

void MockI2C::writeWordSMBus(uint8_t command, uint16_t data) {
    log(LogLevel::Debug) << "Word write (command 0x" << hex << setw(2) << command << setw(0) << ") to SMBus '" << name << "': 0x" << setw(4) << data << setw(0) << ".";
}

uint16_t MockI2C::processCallSMBus(uint8_t command, uint16_t data) {
    log(LogLevel::Debug) << "Process call (command 0x" << hex << setw(2) << command << setw(0) << ") to SMBus '" << name << "': 0x" << setw(4) << data << setw(0) << ".";
    return 0xFFFF;
}

void MockI2C::readBlockSMBus(uint8_t command, uint8_t *data) {
    log(LogLevel::Debug) << "Block read (command 0x" << hex << setw(2) << command << setw(0) << ") from SMBus '" << name << "'.";
}

void MockI2C::writeBlockSMBus(uint8_t command, uint8_t *data, uint8_t length) {
    ostream &os = log(LogLevel::Debug) << "Block write (command 0x" << hex << setw(2) << command << setw(0) << " to SMBUs '" << name << "':";
    for (int i = 0; i < length; ++i) {
        if (i % 8 == 0) {
            os << "\n   ";
        }
        os << " 0x" << setw(2) << data[i] << setw(0);
    }
}

void MockI2C::doTransaction(const vector<pair<uint8_t *, int16_t>> &steps) {
    ostream &os = log(LogLevel::Debug) << "I2C transaction:";
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

MockI2CFactory::MockI2CFactory() : ResourceFactory<I2C>("mock") {
}

shared_ptr<I2C> MockI2CFactory::create(const string &name) {
    return shared_ptr<I2C>(new MockI2C(name));
}

shared_ptr<I2C> I2C::get(const string &name) {
    return ResourceFactory<I2C>::get(name);
}
