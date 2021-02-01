#ifndef SPACEPI_TARGETLIB_LINUX_INSTALLATIONCONFIG_HPP
#define SPACEPI_TARGETLIB_LINUX_INSTALLATIONCONFIG_HPP

#include <string>

namespace spacepi {
    namespace liblinux {
        class InstallationConfig {
            public:
                std::string sourceDir;
                std::string packages;
                std::string components;
                std::string osRelease;
                std::string aptURL;
        };
    }
}

#endif
