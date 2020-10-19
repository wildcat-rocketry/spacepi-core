#include <string>
#include <vector>
#include <spacepi/target/rpi/OSBuilder.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::target::rpi;

int main(int argc, const char **argv) {
    vector<string> args = CommandConfigurable::parse(argc, argv);
    OSBuilder builder(args);
    builder.run();
    return 0;
}
