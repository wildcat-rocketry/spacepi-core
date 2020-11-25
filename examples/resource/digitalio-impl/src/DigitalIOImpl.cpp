#include <functional>
#include <spacepi/example/DigitalIOImpl.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/resource/DigitalIO.hpp>

using namespace std;
using namespace spacepi::example;
using namespace spacepi::log;
using namespace spacepi::resource;

DigitalIOImpl::operator bool() {
    return false;
}

DigitalIO &DigitalIOImpl::operator =(bool value) {
    log(LogLevel::Info) << "Setting value to " << value << ".";
    return *this;
}

enum DigitalIO::Mode DigitalIOImpl::getMode() const noexcept {
    return DigitalIO::Output;
}

void DigitalIOImpl::setMode(enum DigitalIO::Mode mode) {
}

const function<void(void)> &DigitalIOImpl::getISR() const noexcept {
    static function<void(void)> f;
    return f;
}

enum DigitalIO::Edge DigitalIOImpl::getISREdge() const noexcept {
    return DigitalIO::Disable;
}

void DigitalIOImpl::setISR(const function<void(void)> &isr, enum DigitalIO::Edge edge) {
}

void DigitalIOImpl::clearISR() {
}
