#include <cstdlib>
#include <spacepi/example/App.hpp>
#include <spacepi/util/Command.hpp>

using namespace spacepi::example;
using namespace spacepi::util;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    App app(cmd);
    if (cmd.run()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
