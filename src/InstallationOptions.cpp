#include <SpacePi.hpp>
#include <spacepi/liblinux/InstallationOptions.hpp>
#include <string>

using namespace spacepi::liblinux;
using namespace std;

InstallationOptions::InstallationOptions(spacepi::util::Command &cmd): CommandConfigurable("Installtion Options",cmd){
    fromCommand(configFile, "config-file", "XML file path to configure the OS from");
    fromCommand(outFile, "out", "Output image file path");
}

void InstallationOptions::runCommand(){

}

const std::string &InstallationOptions::getConfigFile() const noexcept{
    return configFile;
}

const std::string &InstallationOptions::getOutFile() const noexcept{
    return outFile;
}
