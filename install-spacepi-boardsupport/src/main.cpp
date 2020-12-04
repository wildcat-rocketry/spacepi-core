#include <SpacePi.hpp>

using namespace spacepi::log;

int main(int argc, const char **argv) {
    Logger log("install-spacepi-boardsupport");
    log(LogLevel::Info) << "Hello, world!";
    return 0;
}
