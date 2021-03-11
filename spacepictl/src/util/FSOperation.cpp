#include <cerrno>
#include <cstring>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/FSOperation.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::spacepictl::util;

void FSOperation::handle(const string &msg, int res) {
    if (res < 0) {
        throw EXCEPTION(ResourceException("Error " + msg + ": " + strerror(errno)));
    }
}
