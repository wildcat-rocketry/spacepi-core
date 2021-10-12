#include <stdio.h>
#include <spacepi/Platform.hpp>

void spacepi::Platform::printConsole(const char *msg, int length) noexcept {
    fwrite(msg, 1, length, stdout);
}
