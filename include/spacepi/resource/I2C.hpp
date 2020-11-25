#ifndef SPACEPI_CORE_RESOURCE_I2C_HPP
#define SPACEPI_CORE_RESOURCE_I2C_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/resource/BusTransaction.hpp>

namespace spacepi {
    namespace resource {
        class I2C : public Bus {
            public:
                static std::shared_ptr<I2C> get(const std::string &name);

                virtual int getSpeed() const noexcept = 0;
                virtual void setSpeed(int speed) = 0;

                virtual bool isSMBusErrorCheckingEnabled() const noexcept = 0;
                virtual void enableSMBusErrorChecking() = 0;
                virtual void disableSMBusErrorChecking() = 0;

                virtual void quickWriteSMBus(bool val) = 0;
                virtual uint8_t readByteSMBus() = 0;
                virtual void writeByteSMBus(uint8_t data) = 0;
                virtual uint8_t readByteSMBus(uint8_t command) = 0;
                virtual void writeByteSMBus(uint8_t command, uint8_t data) = 0;
                virtual uint16_t readWordSMBus(uint8_t command) = 0;
                virtual void writeWordSMBus(uint8_t command, uint16_t data) = 0;
                virtual uint16_t processCallSMBus(uint8_t command, uint16_t data) = 0;
                virtual void readBlockSMBus(uint8_t command, uint8_t *data) = 0;
                virtual void writeBlockSMBus(uint8_t command, uint8_t *data, uint8_t length) = 0;

            protected:
                virtual void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps) = 0;
        };
    }
}

#endif
