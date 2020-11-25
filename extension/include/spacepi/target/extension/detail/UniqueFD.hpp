#ifndef SPACEPI_TARGET_RPI_EXTENSION_DETAIL_UNIQUEFD_HPP
#define SPACEPI_TARGET_RPI_EXTENSION_DETAIL_UNIQUEFD_HPP

namespace spacepi {
    namespace target {
        namespace extension {
            namespace detail {
                class UniqueFD {
                    public:
                        UniqueFD(int fd = -1) noexcept;
                        ~UniqueFD() noexcept;

                        UniqueFD(UniqueFD &) = delete;
                        UniqueFD &operator =(UniqueFD &) = delete;

                        UniqueFD(UniqueFD &&move) noexcept;
                        UniqueFD &operator =(UniqueFD &&move) noexcept;

                        UniqueFD &operator =(int fd) noexcept;
                        operator int() const noexcept;
                        operator bool() const noexcept;

                        void release() noexcept;
                        void reset(int fd = -1) noexcept;

                    private:
                        int fd;
                };
            }
        }
    }
}

#endif
