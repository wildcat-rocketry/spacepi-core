#include <string>
#include <boost/format.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost;
using namespace spacepi::util;

Exception::Exception(format message) : message(message.str()) {
}

Exception::Exception(string message) : message(message) {
}

const char *Exception::what() const noexcept {
    return message.c_str();
}

Exception::pointer Exception::getPointer() {
    return boost::current_exception();
}

std::ostream &operator <<(std::ostream &os, const spacepi::util::Exception::pointer &ex) {
    return os << diagnostic_information(ex);
}

#define SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(name) \
    name::name(format message) : Exception(message) { \
    } \
     \
    name::name(string message) : Exception(message) { \
    }
#include <spacepi/util/Exception.hpp>
#undef SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE
