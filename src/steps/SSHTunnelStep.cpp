#include <cstddef>
#include <cstring>
#include <exception>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/SSHTunnelStep.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/InstallationData.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <spacepi/liblinux/SharedProcess.hpp>
#include <spacepi/liblinux/State.hpp>
#include <spacepi/liblinux/SystemCaller.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void SSHTunnelStep::run(InstallationData &data) {
    State &state = data.getData<State>();
    InstallationOptions &opt = data.getData<InstallationOptions>();

    if (state->sshport() > 0) {
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    handle(sock)
        << "Error opening socket: " << SyscallErrorString;
    try {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_addr.s_addr = 0x08080808;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(53);
        handle(connect(sock, (struct sockaddr *) &addr, sizeof(addr)))
            << "Unable to connect to 8.8.8.8:53: " << SyscallErrorString;
        memset(&addr, 0, sizeof(addr));
        socklen_t len = sizeof(addr);
        handle(getsockname(sock, (struct sockaddr *) &addr, &len))
            << "Failed to get local socket info: " << SyscallErrorString;
        state->set_sshport(ntohs(addr.sin_port));
    } catch (const exception &e) {
        close(sock);
        throw;
    }
    close(sock);

    SharedProcess proc(false, false, true, SSH_EXECUTABLE, {
        "-p", to_string(opt.getSSHPort()),
        "-L", "127.0.0.1:" + to_string(state->sshport()) + ":127.0.0.1:2049",
        opt.getConnectSSH(),
        CMAKE_INSTALL_PREFIX "/bin/spacepictl", "remote-id"
    });
    state->set_sshuid(-1);
    state->set_sshgid(-1);
    while (!proc.error().eof()) {
        string word;
        proc.error() >> word;
        if (word.substr(0, 28) == "spacepi-targetlib-linux:UID=") {
            state->set_sshuid(stoi(word.substr(28)));
        } else if (word.substr(0, 28) == "spacepi-targetlib-linux:GID=") {
            state->set_sshgid(stoi(word.substr(28)));
        } else if (word.substr(0, 28) == "spacepi-targetlib-linux:GRP=") {
            size_t start = 28;
            while (start < word.size()) {
                size_t comma = word.find_first_of(',', start);
                state->add_sshgrp(stoi(word.substr(start, comma - start)));
                start = comma + 1;
            }
            break;
        }
    }
    if (state->sshuid() < 0 || state->sshgid() < 0) {
        throw EXCEPTION(ResourceException("Failed to SSH to remote host"));
    }
    data.initData<SharedProcess>(proc);
}
