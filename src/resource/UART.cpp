#include <ios>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/UART.hpp>

using namespace std;
using namespace spacepi::resource;

UART::UART() : iostream(this), last(EOF) {
}

UART::UART(const UART &copy) : iostream(this), last(copy.last) {
}

UART &UART::operator =(const UART &copy) {
    last = copy.last;
    return *this;
}

shared_ptr<UART> UART::get(const string &name) {
    return ResourceFactory<UART>::get(name);
}

streamsize UART::xsgetn(char *buffer, streamsize count) {
    streamsize read = 0;
    if (last >= 0 && count > 0) {
        *buffer++ = last;
        ++read;
        --count;
    }
    if (count > 0) {
        int diff = readBuf(buffer, count);
        if (diff >= 0) {
            read += diff;
        } else {
            return EOF;
        }
    }
    last = EOF;
    return read;
}

streamsize UART::xsputn(char *buffer, streamsize count) {
    return writeBuf(buffer, count);
}

int UART::underflow() {
    if (last >= 0) {
        return last;
    }
    if (readBuf(&(char &) last, 1) == 1) {
        return last;
    }
    return EOF;
}

int UART::uflow() {
    char c;
    if (xsgetn(&c, 1) == 1) {
        return c;
    }
    return EOF;
}