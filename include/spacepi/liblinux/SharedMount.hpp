#ifndef SPACEPI_TARGETLIB_LINUX_SHAREDMOUNT_HPP
#define SPACEPI_TARGETLIB_LINUX_SHAREDMOUNT_HPP

#include <memory>
#include <string>
#include <spacepi/liblinux/UniqueMount.hpp>

namespace spacepi {
    namespace liblinux {
        class SharedMount {
            public:
                SharedMount(const std::string &blockDevice, const std::string &mountPoint, const std::string &options, const std::string &type);
                SharedMount(UniqueMount &&mount);

                bool isMounted() const noexcept;
                void mount();
                void unmount();
                void remount(const std::string &options);

            private:
                std::shared_ptr<UniqueMount> unique;
        };
    }
}

#endif
