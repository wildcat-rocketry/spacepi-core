#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_UART_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_UART_HPP

#include <string>
#include <chrono>
#include <SpacePi.hpp>
#include <spacepi/target/extension/detail/UniqueFD.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class UART : public spacepi::resource::UART {
                public:
                    UART(const std::string &deviceName);

                    int getBAUDRate() const noexcept;
                    void setBAUDRate(int baud);

                private:
                    int readBuf(char *buffer, int count);
                    int writeBuf(const char *buffer, int count);

                    void set_baud(int baud);
                    void throwError(int returnCode);

                    detail::UniqueFD fd;
                    int baud;
                    std::chrono::duration<double> baud_period;
            };
        }
    }
}

#endif
