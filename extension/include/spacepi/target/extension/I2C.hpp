#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_I2C_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_I2C_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/target/extension/detail/UniqueFD.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class I2C : public spacepi::resource::I2C {
                public:
                    I2C(const std::string &deviceName, uint16_t address);

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
                    void throwError(const std::string &action, int returnCode);

                    uint16_t address;
                    detail::UniqueFD fd;
                    bool errorChecking;
            };
        }
    }
}

#endif
