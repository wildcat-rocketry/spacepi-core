#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_UTIL_RWROOT_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_UTIL_RWROOT_HPP

#include <sys/vfs.h>
#include <SpacePi.hpp>

namespace spacepi {
    namespace spacepictl {
        namespace util {
            class RWRoot : private spacepi::log::AutoLog<decltype("spacepictl:rwroot"_autolog)> {
                public:
                    RWRoot();
                    ~RWRoot() noexcept;

                    RWRoot(RWRoot &) = delete;
                    RWRoot &operator =(RWRoot &) = delete;

                private:
                    struct statfs stat;
            };
        }
    }
}

#endif
