#include <spacepi/Platform.hpp>
#include <stdio.h>

void spacepi::Platform::printConsole(const char *msg, int length) noexcept {
    fwrite(msg, 1, length, stdout);
}