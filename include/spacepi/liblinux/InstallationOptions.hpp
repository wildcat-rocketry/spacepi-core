#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONOPTIONS_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONOPTIONS_HPP

#include <SpacePi.hpp>
#include <string>

namespace spacepi {
    namespace liblinux {
        class InstallationOptions : public spacepi::util::CommandConfigurable {
            public:
                InstallationOptions(spacepi::util::Command &cmd);

                void runCommand();

                const std::string &getConfigFile() const noexcept;
                const std::string &getOutFile() const noexcept;
                const std::string &getConnectSSH() const noexcept;
                int getSSHPort() const noexcept;

            private:
                std::string configFile;
                std::string outFile;
                std::string connectSSH;
                int sshPort;
        };
    }
}

#endif
