#include <cerrno>
#include <cstring>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::detail;

void SyscallErrorStreamImpl::doThrow() {
    std::string val = str();
    if (val.empty()) {
        val = "Unhandled error running syscall.";
    }
    throw EXCEPTION(ResourceException(val));
}

SyscallErrorStream::SyscallErrorStream(bool error) noexcept : err(errno) {
    if (error) {
        stream.reset(new SyscallErrorStreamImpl());
    }
}

SyscallErrorStream::~SyscallErrorStream() noexcept(false) {
    if (stream.unique()) {
        stream->doThrow();
    }
}

SyscallErrorStream &SyscallErrorStream::operator <<(SyscallData data) noexcept {
    switch (data) {
        case SyscallErrorString:
            return *this << strerror(err);
        case SyscallErrorNo:
            return *this << err;
        default:
            return *this << "(unknown error field " << data << ")";
    }
}

SyscallErrorStream SystemCaller::handle(int res) const {
    return SyscallErrorStream(res < 0);
}
