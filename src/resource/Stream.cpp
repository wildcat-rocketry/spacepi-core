#include <ios>
#include <istream>
#include <streambuf>
#include <spacepi/resource/Stream.hpp>

using namespace std;
using namespace spacepi::resource;

Stream::Stream() : iostream(this), last(EOF) {
}

Stream::Stream(const Stream &copy) : iostream(this), last(copy.last) {
}

Stream &Stream::operator =(const Stream &copy) {
    last = copy.last;
    return *this;
}

streamsize Stream::xsgetn(char *buffer, streamsize count) {
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

streamsize Stream::xsputn(char *buffer, streamsize count) {
    return writeBuf(buffer, count);
}

int Stream::underflow() {
    if (last >= 0) {
        return last;
    }
    if (readBuf(&(char &) last, 1) == 1) {
        return last;
    }
    return EOF;
}

int Stream::uflow() {
    char c;
    if (xsgetn(&c, 1) == 1) {
        return c;
    }
    return EOF;
}
