#ifndef SPACEPI_TARGETLIB_LINUX_FSOSTREAM_HPP
#define SPACEPI_TARGETLIB_LINUX_FSOSTREAM_HPP

#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <spacepi/spacepictl/FSOperation.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>

namespace spacepi {
    namespace spacepictl {
        namespace detail {
            class FSOStreamBuf : public std::stringbuf, public FSOperation {
                public:
                    FSOStreamBuf(const std::string &filename, uid_t uid, gid_t gid, mode_t mode, bool keepBackup);

                private:
                    void perform();
                    void undo();
                    void finalize();

                    std::string filename;
                    uid_t uid;
                    gid_t gid;
                    mode_t mode;
                    bool keepBackup;
                    bool hasBackup;
                    bool wrote;
            };
        }

        class FSOStream : public std::ostream {
            public:
                FSOStream(FSTransaction &transaction, const std::string &filename, uid_t uid = 0, gid_t gid = 0, mode_t mode = 0644, bool keepBackup = false);
        };
    }
}

#endif
