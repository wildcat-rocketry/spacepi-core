#ifndef SPACEPI_CORE_RESOURCE_BUSTRANSACTION_HPP
#define SPACEPI_CORE_RESOURCE_BUSTRANSACTION_HPP

#include <cstdint>
#include <utility>
#include <vector>

namespace spacepi {
    namespace resource {
        class BusTransaction;

        class Bus {
            friend class BusTransaction;

            public:
                BusTransaction operator <<(uint8_t &data);
                BusTransaction operator <<(uint16_t &data);
                BusTransaction operator <<(uint32_t &data);

                BusTransaction operator >>(uint8_t &data);
                BusTransaction operator >>(uint16_t &data);
                BusTransaction operator >>(uint32_t &data);

                BusTransaction read(uint8_t *data, uint16_t length);
                BusTransaction write(uint8_t *data, uint16_t length);

            private:
                virtual void doTransaction(const std::vector<std::pair<uint8_t *, int16_t>> &steps) = 0;
        };

        class BusTransaction {
            public:
                BusTransaction(Bus &bus);
                ~BusTransaction();

                BusTransaction(BusTransaction &copy);
                BusTransaction &operator =(BusTransaction &copy);

                BusTransaction &operator <<(uint8_t &data);
                BusTransaction &operator <<(uint16_t &data);
                BusTransaction &operator <<(uint32_t &data);

                BusTransaction &operator >>(uint8_t &data);
                BusTransaction &operator >>(uint16_t &data);
                BusTransaction &operator >>(uint32_t &data);

                BusTransaction &read(uint8_t *data, uint16_t length);
                BusTransaction &write(uint8_t *data, uint16_t length);

            private:
                Bus &bus;
                std::vector<std::pair<uint8_t *, int16_t>> steps;
        };
    }
}

#endif
