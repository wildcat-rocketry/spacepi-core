#include <cstdlib>
#include <spacepi/example/ClassA.hpp>
#include <spacepi/example/ClassB.hpp>
#include <spacepi/util/Command.hpp>

using namespace spacepi::example;
using namespace spacepi::util;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    ClassA a(cmd);
    ClassB b(cmd);
    if (cmd.run()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
