#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONCONFIG_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONCONFIG_HPP

#include <string>
#include <vector>
#include <unistd.h>

namespace spacepi {
    namespace liblinux {
        class InstallationConfig {
            public:
                std::string sourceDir;
                std::vector<std::string> nfsDirs;
                std::string packages;
                std::string components;
                std::string osRelease;
                std::string aptURL;
                std::string dpkgArch;
                std::vector<std::string> emulationBin;
                uid_t sourceUid;
                gid_t sourceGid;
        };
    }
}

#endif
