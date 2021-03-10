#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <spacepi/resource/DigitalIO.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<DigitalIO> DigitalIO::get(const string &name) {
    return ResourceFactory<DigitalIO>::get(name);
}

void DigitalIO::clearISR() {
    static function<void(void)> nil;
    setISR(nil, Disable);
}

enum DigitalIO::Mode operator |(enum DigitalIO::Mode a, enum DigitalIO::Mode b) noexcept {
    return (enum DigitalIO::Mode) (a | b);
}

enum DigitalIO::Mode &operator |=(enum DigitalIO::Mode &a, enum DigitalIO::Mode b) noexcept {
    return a = (a | b);
}

ostream &operator <<(ostream &os, DigitalIO::Mode mode) {
    switch (mode) {
        case DigitalIO::Output:
            return os << "Output";
        case DigitalIO::Output | (int) DigitalIO::OpenDrain:
            return os << "Output (Open Drain)";
        case DigitalIO::Output | (int) DigitalIO::OpenSource:
            return os << "Output (Open Source)";
        case DigitalIO::Input:
            return os << "Input";
        case DigitalIO::Input | (int) DigitalIO::PullUp:
            return os << "Input (with Pullup)";
        case DigitalIO::Input | (int) DigitalIO::PullDown:
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
