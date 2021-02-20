#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUELOOPDEVICE_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUELOOPDEVICE_HPP

#include <string>

namespace spacepi {
    namespace liblinux {
        class UniqueLoopDevice {
            public:
                UniqueLoopDevice(const std::string &imageFile);
                ~UniqueLoopDevice() noexcept(false);

                UniqueLoopDevice(UniqueLoopDevice &) = delete;
                UniqueLoopDevice &operator =(UniqueLoopDevice &) = delete;

                UniqueLoopDevice(UniqueLoopDevice &&move) noexcept;
                UniqueLoopDevice &operator =(UniqueLoopDevice &&move) noexcept;

                std::string getBlockDevice(int partNo) const;

                bool isMounted() const noexcept;
                void mount();
                void unmount();
                void forceUnmount();

            private:
                std::string imageFile;
                std::string baseDevice;
                bool mounted;
        };
    }
}

#endif
