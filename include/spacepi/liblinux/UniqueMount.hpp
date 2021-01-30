#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUEMOUNT_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUEMOUNT_HPP

#include <string>
#include <spacepi/liblinux/SystemCaller.hpp>

namespace spacepi {
    namespace liblinux {
        class UniqueMount : private SystemCaller {
            public:
                UniqueMount(const std::string &blockDevice, const std::string &mountPoint, const std::string &options, const std::string &type);
                ~UniqueMount();

                UniqueMount(UniqueMount &) = delete;
                UniqueMount &operator =(UniqueMount &) = delete;

                UniqueMount(UniqueMount &&move) noexcept;
                UniqueMount &operator =(UniqueMount &&move) noexcept;

                bool isMounted() const noexcept;
                void mount();
                void unmount();

            private:
                std::string blockDevice;
                std::string mountPoint;
                unsigned long flags;
                std::string options;
                std::string type;
                bool mounted;
        };
    }
}

#endif
