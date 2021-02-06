#ifndef SPACEPI_TARGETLIB_LINUX_FSTRANSACTION_HPP
#define SPACEPI_TARGETLIB_LINUX_FSTRANSACTION_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSOperation.hpp>

namespace spacepi {
    namespace spacepictl {
        namespace detail {
            class FSGitConfigOperation : public FSOperation {
                public:
                    FSGitConfigOperation(const std::string &path, const std::map<std::string, std::string> &confEntries, uid_t uid, gid_t gid, mode_t mode);

                private:
                    void perform();
                    void undo();
                    void finalize();
                    void git_handle(const std::string &msg, int ret);

                    std::string path;
                    gid_t gid;
                    uid_t uid;
                    mode_t mode;

                    bool hasBackup;
                    bool wrote;

                    std::map<std::string, std::string> confEntries;
            };

            class FSMkSymlinkOperation : public FSOperation {
                public:
                    FSMkSymlinkOperation(const std::string &path, const std::string &target, uid_t uid, gid_t gid, mode_t mode);

                private:
                    void perform();
                    void undo();
                    void finalize();

                    std::string target;
                    std::string path;
                    gid_t gid;
                    uid_t uid;
                    mode_t mode;
                    bool hasBackup;
                    bool wrote;

                    bool didExist;
            };

            class FSRemoveOperation : public FSOperation {
                public:
                    FSRemoveOperation(const std::string &file);

                private:
                    void perform();
                    void undo();
                    void finalize();

                    std::string file;
                    bool didExist;
            };

            class FSMkdirOperation : public FSOperation {
                public:
                    FSMkdirOperation(const std::string &path, uid_t uid, gid_t gid, mode_t mode);

                private:
                    void perform();
                    void undo();
                    void finalize();

                    std::string path;
                    uid_t uid;
                    uid_t oldUid;
                    gid_t gid;
                    gid_t oldGid;
                    mode_t mode;
                    mode_t oldMode;
                    bool made;
                    bool setOwn;
                    bool setMod;
            };

            class FSCopyOperation : public FSOperation {
                public:
                    FSCopyOperation(const std::string &from, const std::string &to, uid_t uid, gid_t gid, mode_t mode);

                private:
                    void perform();
                    void undo();
                    void finalize();

                    std::string from;
                    std::string to;
                    uid_t uid;
                    gid_t gid;
                    mode_t mode;
                    bool hasBackup;
                    bool wrote;
            };
        }

        class FSTransaction : spacepi::log::AutoLog<decltype("spacepictl:fs"_autolog)> {
            public:
                FSTransaction() noexcept;

                FSTransaction(FSTransaction &) = delete;
                FSTransaction &operator =(FSTransaction &) = delete;

                void apply();
                FSTransaction &operator +=(const std::shared_ptr<FSOperation> &op);

                void remove(const std::string &file);
                void mkdir(const std::string &path, uid_t uid = 0, gid_t gid = 0, mode_t mode = 0755);
                void copy(const std::string &from, const std::string &to, uid_t uid = 0, gid_t gid = 0, mode_t mode = 0644);
                void link(const std::string &path, const std::string &target, uid_t uid = 0, gid_t gid = 0, mode_t mode = 0644);
                void add_git_config(const std::string &path, std::map<std::string,std::string> confEntries, uid_t uid = 0, gid_t gid = 0, mode_t mode = 0644);

            private:
                bool applied;
                std::vector<std::shared_ptr<FSOperation>> ops;
        };
    }
}

#endif
