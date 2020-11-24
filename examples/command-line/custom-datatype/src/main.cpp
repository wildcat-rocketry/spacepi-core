#include <cstdlib>
#include <spacepi/example/CustomOption.hpp>
#include <spacepi/util/Command.hpp>

using namespace std;
using namespace spacepi::example;
using namespace spacepi::util;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    CustomOption opt(cmd);
    if (cmd.run()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
