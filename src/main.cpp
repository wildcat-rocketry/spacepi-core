#include <cstdlib>
#include <SpacePi.hpp>
#include <spacepi/target/OSBuilder.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::target;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    OSBuilder builder(cmd);
    if (cmd.run()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
