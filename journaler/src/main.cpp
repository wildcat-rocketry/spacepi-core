#include <cstdlib>
#include <memory>
#include <spacepi/util/Command.hpp>
#include <spacepi/journaler/FullConnection.hpp>
#include <spacepi/journaler/Journal.hpp>
#include <spacepi/journaler/Journaler.hpp>
#include <spacepi/journaler/Playback.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::journaler;

int main(int argc, const char **argv) {
    Command cmd(argc, argv);
    Journal journal(cmd);
    shared_ptr<FullConnection> conn(new FullConnection(cmd));
    Playback playback(cmd, journal, *conn);
    Journaler journaler(cmd, journal, *conn);
    if (cmd.run()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
