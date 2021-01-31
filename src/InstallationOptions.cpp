#include <SpacePi.hpp>
#include <string>
#include <spacepi/liblinux/InstallationOptions.hpp>

using namespace spacepi::liblinux;
using namespace std;

InstallationOptions::InstallationOptions(spacepi::util::Command &cmd): CommandConfigurable("Installtion Options",cmd){
    fromCommand(configFile, "config-file", "XML file path to configure the OS from");
    fromCommand(outFile, "out", "Output image file path");
    fromCommand(dataDir, "data-dir", "The source directory of the OS target to get data files from");
}

void InstallationOptions::runCommand(){

}

const std::string &InstallationOptions::getConfigFile() const noexcept{
    return configFile;
}

const std::string &InstallationOptions::getOutFile() const noexcept{
    return outFile;
}

const std::string &InstallationOptions::getDataDir() const noexcept{
    return dataDir;
}
