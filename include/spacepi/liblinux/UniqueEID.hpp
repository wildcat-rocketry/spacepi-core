#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUEEID_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUEEID_HPP

#include <unistd.h>
#include <spacepi/liblinux/SystemCaller.hpp>

namespace spacepi {
    namespace liblinux {
        class UniqueEID : private SystemCaller {
            public:
                UniqueEID(uid_t uid, gid_t gid);
                ~UniqueEID() noexcept(false);

                UniqueEID(UniqueEID &) = delete;
                UniqueEID &operator =(UniqueEID &) = delete;

                UniqueEID(UniqueEID &&move) noexcept;
                UniqueEID &operator =(UniqueEID &&move) noexcept;

                bool isActive() const noexcept;
                void setId();
                void unsetId();
                void forceUnsetId();

            private:
                uid_t oldUid;
                gid_t oldGid;
                uid_t newUid;
                gid_t newGid;
                bool active;
        };
    }
}

#endif
