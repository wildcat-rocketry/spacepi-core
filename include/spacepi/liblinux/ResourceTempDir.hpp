#ifndef SPACEPI_TARGETLIB_LINUX_RESOURCETEMPDIR_HPP
#define SPACEPI_TARGETLIB_LINUX_RESOURCETEMPDIR_HPP

#include <string>
#include <spacepi/liblinux/SharedTempDir.hpp>

namespace spacepi {
    namespace liblinux {
        class ResourceTempDir : public SharedTempDir {
            public:
                ResourceTempDir(const std::string &name);
        };
    }
}

#endif
