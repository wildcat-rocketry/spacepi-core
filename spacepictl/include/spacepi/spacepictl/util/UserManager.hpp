#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_USER_USERMANAGER_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_USER_USERMANAGER_HPP

#include <string>
#include <list>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

#include <boost/property_tree/ptree.hpp>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/FSTransaction.hpp>
#include <spacepi/spacepictl/util/User.hpp>
#include <spacepi/spacepictl/util/Person.hpp>

namespace spacepi {
    namespace spacepictl {
        namespace util {
            class UserManager : private spacepi::log::AutoLog<decltype("spacepictl:usermanager"_autolog)> {
                public:
                    UserManager(spacepi::spacepictl::util::FSTransaction &fs, const std::unordered_map<std::string, spacepi::package::User> &users);

                    void write_users();

                protected:
                    bool update;

                private:
                    spacepi::spacepictl::util::FSTransaction & fs;

                    std::list<spacepi::spacepictl::util::User> system_users;
                    std::list<spacepi::spacepictl::util::Person> human_users;
                    spacepi::spacepictl::util::Person create_person(const struct passwd * cur_pwd, const struct spwd * cur_spwd, std::string uname, gid_t sudo_gid);

                    std::list<uid_t> uids;
                    uid_t next_uid(uid_t start);
            };
        }
    }
}

#endif
