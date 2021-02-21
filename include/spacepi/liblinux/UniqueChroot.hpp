#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUECHROOT_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUECHROOT_HPP

#include <string>
#include <spacepi/liblinux/SystemCaller.hpp>

namespace spacepi {
    namespace liblinux {
        class UniqueChroot : private SystemCaller {
            public:
                UniqueChroot(const std::string &root);
                ~UniqueChroot() noexcept(false);

                UniqueChroot(UniqueChroot &) = delete;
                UniqueChroot &operator =(UniqueChroot &) = delete;

                UniqueChroot(UniqueChroot &&move) noexcept;
                UniqueChroot &operator =(UniqueChroot &&move) noexcept;

                bool isActive() const noexcept;
                void enter();
                void escape();
                void forceEscape();

            private:
                std::string root;
                int realRoot;
                int oldCwd;
                bool active;
        };
    }
}

#endif
