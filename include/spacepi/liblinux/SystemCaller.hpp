#ifndef SPACEPI_TARGETLIB_LINUX_SYSTEMCALLER_HPP
#define SPACEPI_TARGETLIB_LINUX_SYSTEMCALLER_HPP

#include <memory>
#include <sstream>
#include <string>

namespace spacepi {
    namespace liblinux {
        namespace detail {
            class SyscallErrorStreamImpl : public std::ostringstream {
                public:
                    ~SyscallErrorStreamImpl();
            };
        }

        enum SyscallData {
            SyscallErrorString,
            SyscallErrorNo
        };

        class SyscallErrorStream {
            public:
                SyscallErrorStream(bool error) noexcept;

                template <typename Type>
                SyscallErrorStream &operator <<(Type data) noexcept {
                    if (stream) {
                        *stream << data;
                    }
                    return *this;
                }

                SyscallErrorStream &operator <<(SyscallData data) noexcept;

            private:
                int err;
                std::shared_ptr<detail::SyscallErrorStreamImpl> stream;
        };

        class SystemCaller {
            protected:
                SyscallErrorStream handle(int res) const;
        };
    }
}

#endif
