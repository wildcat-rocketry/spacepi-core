#ifndef SPACEPI_TARGETLIB_LINUX_SHAREDLOOPDEVICE_HPP
#define SPACEPI_TARGETLIB_LINUX_SHAREDLOOPDEVICE_HPP

#include <memory>
#include <string>
#include <spacepi/liblinux/UniqueLoopDevice.hpp>

namespace spacepi {
    namespace liblinux {
        class SharedLoopDevice {
            public:
                SharedLoopDevice(const std::string &imageFile);
                SharedLoopDevice(UniqueLoopDevice &&loop);

                std::string getBlockDevice(int partNo) const;

                bool isMounted() const noexcept;
                void mount();
                void unmount();

            private:
                std::shared_ptr<UniqueLoopDevice> unique;
        };
    }
}

#endif
