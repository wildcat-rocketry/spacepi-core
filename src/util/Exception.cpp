#include <csignal>
#include <string>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace spacepi::util;

static void segmentationFaultHandler(int sig);

static void (*segmentationFaultRegistration)(int) = signal(SIGSEGV, segmentationFaultHandler);

Exception::Exception(const string &message) noexcept : message(message) {
}

const char *Exception::what() const noexcept {
    return message.c_str();
}

Exception::pointer Exception::getPointer() noexcept {
    return boost::current_exception();
}

string Exception::what(Exception::pointer ptr) noexcept {
    try {
        rethrow_exception(ptr);
    } catch (const std::exception &ex) {
        return string(ex.what());
    }
    return "(unknown exception)";
}

std::ostream &operator <<(std::ostream &os, const spacepi::util::Exception::pointer &ex) {
    return os << diagnostic_information(ex);
}

static void segmentationFaultHandler(int sig) {
    throw EXCEPTION(SegmentationFaultException("Attempt to access unallocated memory"));
}

#define SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(name) \
    name::name(const string &message) noexcept : Exception(message) { \
    }
#include <spacepi/util/Exception.hpp>
#undef SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE
