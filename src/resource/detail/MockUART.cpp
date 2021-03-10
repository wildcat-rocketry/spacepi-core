#include <iomanip>
#include <ios>
#include <memory>
#include <streambuf>
#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogStream.hpp>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/UART.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockUART : public UART, private spacepi::log::AutoLog<decltype("core:resource:mock:uart"_autolog)> {
                public:
                    MockUART(const std::string &name) noexcept;

                    int getBAUDRate() const noexcept;
                    void setBAUDRate(int baud);

                protected:
                    std::streamsize xsgetn(char *buffer, std::streamsize count);
                    std::streamsize xsputn(char *buffer, std::streamsize count);

                private:
                    std::string name;
                    int baud;
            };

            class MockUARTFactory : public ResourceFactory<UART> {
                public:
                    static MockUARTFactory instance;

                    MockUARTFactory();

                private:
                    std::shared_ptr<UART> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockUARTFactory MockUARTFactory::instance;

MockUART::MockUART(const string &name) noexcept : name(name), baud(115200) {
    log(LogLevel::Debug) << "Created new UART '" << name << "'.";
}

int MockUART::getBAUDRate() const noexcept {
    return baud;
}

void MockUART::setBAUDRate(int baud) {
    log(LogLevel::Debug) << "Setting UART '" << name << "' BAUD rate to " << baud << ".";
    this->baud = baud;
}

streamsize MockUART::xsgetn(char *buffer, streamsize count) {
    log(LogLevel::Debug) << "Reading UART '" << name << "': " << count << " bytes";
    return count;
}

streamsize MockUART::xsputn(char *buffer, streamsize count) {
    LogStream os = log(LogLevel::Debug);
    os << "Writing UART '" << name << "': " << count << "bytes:" << hex << setfill('0');
    for (int i = 0; i < count; ++i) {
        if (i % 8 == 0) {
            os << "\n   ";
        }
        os << " 0x" << setw(2) << (int) buffer[i] << setw(0);
    }
    return count;
}

MockUARTFactory::MockUARTFactory() : ResourceFactory<UART>("mock") {
}

shared_ptr<UART> MockUARTFactory::create(const string &name) {
    return shared_ptr<UART>(new MockUART(name));
}
