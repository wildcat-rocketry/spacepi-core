#include <spacepi/dtc/main/Main.hpp>

using namespace spacepi::dtc::main;

int main(int argc, const char **argv) noexcept {
    return Main::instance->run(argc, argv);
}
