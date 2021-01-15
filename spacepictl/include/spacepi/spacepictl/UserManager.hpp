#ifndef SPACEPI_TARGET_RPI_USERMANAGER_HPP
#define SPACEPI_TARGET_RPI_USERMANAGER_HPP

#include <string>
#include <vector>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

#include <boost/property_tree/ptree.hpp>
#include <spacepi/spacepictl/User.hpp>
#include <spacepi/spacepictl/Person.hpp>

namespace spacepi {
    namespace spacepictl {
            class UserManager {
                public:
                    UserManager(boost::property_tree::ptree & users);

                    bool needs_update();

                    void write_users();

                protected:
                    bool update;

                private:
                    std::list<spacepi::spacepictl::User> system_users;
                    std::list<spacepi::spacepictl::Person> human_users;
                    spacepi::spacepictl::Person create_person(const struct passwd * cur_pwd, const struct spwd * cur_spwd, std::string uname, gid_t sudo_gid);

                    std::list<uid_t> uids;
                    uid_t next_uid(uid_t start);
            };
        }
}

#endif
