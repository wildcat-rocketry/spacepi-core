#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/UART.hpp>

using namespace std;
using namespace spacepi::resource;

UART::UART() : iostream(this) {
}

UART::UART(const UART &copy) : iostream(this) {
}

UART &UART::operator =(const UART &copy) {
    return *this;
}

shared_ptr<UART> UART::get(const string &name) {
    return ResourceFactory<UART>::get(name);
}

int UART::underflow() {
    char buff[1];
    int len = xsgetn(buff, 1);
    if(len == 0) return EOF;
    else {
        last = buff[0];
        return last;
    }
}

int UART::uflow() {
    char c;
    if (xsgetn(&c, 1) == 1) {
        return c;
    }
    return EOF;
}