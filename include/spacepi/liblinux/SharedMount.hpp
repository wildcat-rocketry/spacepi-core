#ifndef SPACEPI_TARGETLIB_LINUX_SHAREDMOUNT_HPP
#define SPACEPI_TARGETLIB_LINUX_SHAREDMOUNT_HPP

#include <memory>
#include <string>
#include <vector>
#include <spacepi/liblinux/UniqueMount.hpp>

namespace spacepi {
    namespace liblinux {
        class SharedMount {
            public:
                SharedMount(const std::string &blockDevice, const std::string &mountPoint, const std::string &options, const std::string &type);
                SharedMount(UniqueMount &&mount);
                ~SharedMount() noexcept(false);

                SharedMount(const SharedMount &) noexcept = default;
                SharedMount &operator =(const SharedMount &) noexcept = default;

                bool isMounted() const noexcept;
                void mount();
                void unmount();
                void forceUnmount();
                void remount(const std::string &options);

            private:
                std::shared_ptr<UniqueMount> unique;
        };

        namespace detail {
            class SharedMountAllocator : public std::allocator<SharedMount> {
                public:
                    SharedMountAllocator() noexcept = default;
                    SharedMountAllocator(const SharedMountAllocator &) noexcept = default;

                    template <typename Type>
                    SharedMountAllocator(const std::allocator<Type> &alloc) : std::allocator<SharedMount>(alloc) {
                    }
            };
        }
    }
}

namespace std {
    template <>
    class vector<spacepi::liblinux::SharedMount> : public vector<spacepi::liblinux::SharedMount, spacepi::liblinux::detail::SharedMountAllocator> {
        public:
            vector() noexcept = default;
            vector(const vector<spacepi::liblinux::SharedMount> &copy) noexcept;
            vector<spacepi::liblinux::SharedMount> &operator =(const vector<spacepi::liblinux::SharedMount> &) = default;
            vector(vector<spacepi::liblinux::SharedMount> &&move) noexcept;
            vector<spacepi::liblinux::SharedMount> &operator =(vector<spacepi::liblinux::SharedMount> &&) = default;

            ~vector() noexcept(false);
    };
}

#endif
