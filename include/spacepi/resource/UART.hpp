#ifndef SPACEPI_CORE_RESOURCE_UART_HPP
#define SPACEPI_CORE_RESOURCE_UART_HPP

#include <istream>
#include <memory>
#include <streambuf>
#include <string>

namespace spacepi {
    namespace resource {
        class UART : public std::iostream, public std::streambuf {
            public:
                UART();

                UART(const UART &copy);
                UART &operator =(const UART &copy);

                static std::shared_ptr<UART> get(const std::string &name);

                virtual int getBAUDRate() const noexcept = 0;
                virtual void setBAUDRate(int baud) = 0;

            protected:
                virtual std::streamsize xsgetn(char *buffer, std::streamsize count) = 0;
                virtual std::streamsize xsputn(char *buffer, std::streamsize count) = 0;
        };
    }
}

#endif
