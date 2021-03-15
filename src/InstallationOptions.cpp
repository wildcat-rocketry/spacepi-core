#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <string>

using namespace spacepi::liblinux;
using namespace std;

InstallationOptions::InstallationOptions(spacepi::util::Command &cmd): CommandConfigurable("Installtion Options",cmd){
    fromCommand(configFile, "", "config-file", "XML file path to configure the OS from");
    fromCommand(outFile, "out", "Output image file path");
    fromCommand(connectSSH, "", "connect", "Connect to a running machine over SSH/NFS");
    fromCommand(sshPort, 22, "port", "Port for SSH connection (see --connect)");
    fromCommand(state, "", "state", "Internal state data");
}

void InstallationOptions::runCommand(){

}

const std::string &InstallationOptions::getConfigFile() const noexcept{
    return configFile;
}

const std::string &InstallationOptions::getOutFile() const noexcept{
    return outFile;
}

const string &InstallationOptions::getConnectSSH() const noexcept {
    return connectSSH;
}

int InstallationOptions::getSSHPort() const noexcept {
    return sshPort;
}

const string &InstallationOptions::getState() const noexcept {
    return state;
}
