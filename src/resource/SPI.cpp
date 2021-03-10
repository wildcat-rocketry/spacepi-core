#include <memory>
#include <ostream>
#include <string>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/SPI.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<SPI> SPI::get(const string &name) {
    return ResourceFactory<SPI>::get(name);
}

ostream &operator <<(ostream &os, enum SPI::Mode mode) {
    switch (mode) {
        case SPI::Mode0 | SPI::MSBFirst:
            return os << "Mode 0 (Falling Edge, Active High), MSB First";
        case SPI::Mode1 | SPI::MSBFirst:
            return os << "Mode 1 (Rising Edge, Active High), MSB First";
        case SPI::Mode2 | SPI::MSBFirst:
            return os << "Mode 2 (Falling Edge, Active Low), MSB First";
        case SPI::Mode3 | SPI::MSBFirst:
            return os << "Mode 3 (Rising Edge, Active Low), MSB First";
        case SPI::Mode0 | SPI::LSBFirst:
            return os << "Mode 0 (Falling Edge, Active High), LSB First";
        case SPI::Mode1 | SPI::LSBFirst:
            return os << "Mode 1 (Rising Edge, Active High), LSB First";
        case SPI::Mode2 | SPI::LSBFirst:
            return os << "Mode 2 (Falling Edge, Active Low), LSB First";
        case SPI::Mode3 | SPI::LSBFirst:
            return os << "Mode 3 (Rising Edge, Active Low), LSB First";
        default:
            return os << "Unknown mode " << (int) mode;
    }
}
