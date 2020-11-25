#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/DigitalIO.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockDigitalIO : public DigitalIO, private spacepi::log::AutoLog<decltype("core:resource:mock:digitalio"_autolog)> {
                public:
                    MockDigitalIO(const std::string &name) noexcept;

                    operator bool();
                    DigitalIO &operator =(bool value);

                    enum Mode getMode() const noexcept;
                    void setMode(enum Mode mode);

                    const std::function<void(void)> &getISR() const noexcept;
                    enum Edge getISREdge() const noexcept;
                    void setISR(const std::function<void(void)> &isr, enum Edge edge);
                    void clearISR();

                private:
                    std::string name;
                    bool value;
                    enum Mode mode;
                    const std::function<void(void)> *isr;
                    enum Edge isrEdge;
            };

            class MockDigitalIOFactory : public ResourceFactory<DigitalIO> {
                public:
                    static MockDigitalIOFactory instance;

                    MockDigitalIOFactory();

                private:
                    std::shared_ptr<DigitalIO> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockDigitalIOFactory MockDigitalIOFactory::instance;

MockDigitalIO::MockDigitalIO(const string &name) noexcept : name(name), value(false), mode(DigitalIO::Input), isr(nullptr), isrEdge(DigitalIO::Disable) {
    log(LogLevel::Debug) << "Created new DigitalIO '" << name << "'.";
    if (!name.empty() && name.front() == '!') {
        value = true;
    }
}

MockDigitalIO::operator bool() {
    log(LogLevel::Debug) << "Reading Digital IO '" << name << "'.";
    return value;
}

DigitalIO &MockDigitalIO::operator =(bool value) {
    log(LogLevel::Debug) << "Writing Digital IO '" << name << "' to " << value << ".";
    this->value = value;
    return *this;
}

enum DigitalIO::Mode MockDigitalIO::getMode() const noexcept {
    return mode;
}

void MockDigitalIO::setMode(enum DigitalIO::Mode mode) {
    log(LogLevel::Debug) << "Setting Digital IO '" << name << "' mode to " << mode << ".";
    this->mode = mode;
}

const function<void(void)> &MockDigitalIO::getISR() const noexcept {
    if (isr) {
        return *isr;
    }
    static function<void(void)> nil;
    return nil;
}

enum DigitalIO::Edge MockDigitalIO::getISREdge() const noexcept {
    return isrEdge;
}

void MockDigitalIO::setISR(const function<void(void)> &isr, enum DigitalIO::Edge edge) {
    log(LogLevel::Debug) << "Setting Digital IO '" << name << "' ISR to trigger on " << edge << ".";
    this->isr = &isr;
    isrEdge = edge;
}

void MockDigitalIO::clearISR() {
    log(LogLevel::Debug) << "Clearing Digital IO '" << name << "' ISR.";
    isr = nullptr;
    isrEdge = Disable;
}

MockDigitalIOFactory::MockDigitalIOFactory() : ResourceFactory<DigitalIO>("mock") {
}

shared_ptr<DigitalIO> MockDigitalIOFactory::create(const string &name) {
    return shared_ptr<DigitalIO>(new MockDigitalIO(name));
}

shared_ptr<DigitalIO> DigitalIO::get(const string &name) {
    return ResourceFactory<DigitalIO>::get(name);
}

ostream &operator <<(ostream &os, DigitalIO::Mode mode) {
    switch (mode) {
        case DigitalIO::Output:
            return os << "Output";
        case DigitalIO::Input:
            return os << "Input";
        case DigitalIO::Input | DigitalIO::PullUp:
            return os << "Input (with Pullup)";
        case DigitalIO::Input | DigitalIO::PullDown:
            return os << "Input (with Pulldown)";
        default:
            return os << "Unknown mode " << (int) mode;
    }
}

ostream &operator <<(ostream &os, DigitalIO::Edge edge) {
    switch (edge) {
        case DigitalIO::Disable:
            return os << "Disable";
        case DigitalIO::Rising:
            return os << "Rising Edge";
        case DigitalIO::Falling:
            return os << "Falling Edge";
        case DigitalIO::Transition:
            return os << "Either Edge";
        default:
            return os << "Unknown edge " << (int) edge;
    }
}
