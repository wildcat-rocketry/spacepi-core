#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONOPTIONS_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONOPTIONS_HPP

#include <string>
#include <SpacePi.hpp>

namespace spacepi {
    namespace liblinux {
        class InstallationOptions : public spacepi::util::CommandConfigurable {
            public:
                InstallationOptions(spacepi::util::Command &cmd);

                void runCommand();

                const std::string &getConfigFile() const noexcept;
                const std::string &getOutFile() const noexcept;
                const std::string &getDataDir() const noexcept;

            private:
                std::string configFile;
                std::string outFile;
                std::string dataDir;
        };
    }
}

#endif
