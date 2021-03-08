#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_UART_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_UART_HPP

#include <streambuf>
#include <string>
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

                protected:
                    std::streamsize xsgetn(char *buffer, std::streamsize count);
                    std::streamsize xsputn(char *buffer, std::streamsize count);
                    int uflow();
                    int underflow();

                private:
                    void throwError(int returnCode);

                    detail::UniqueFD fd;
                    int baud;
            };
        }
    }
}

#endif
