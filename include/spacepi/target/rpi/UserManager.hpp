#ifndef SPACEPI_TARGET_RPI_USERMANAGER_HPP
#define SPACEPI_TARGET_RPI_USERMANAGER_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

#include <spacepi/target/rpi/User>
#include <spacepi/target/rpi/Person>

namespace spacepi {
    namespace target {
        namespace rpi {
            class UserManager {
                public:
                    UserManager(boost::property_tree::ptree & users);

                    // Iterate through user entries and build People List
                    void MakePeopleList(boost::property_tree::ptree & users);

                    // Make a list of the system users
                    void MakeUserList();

                    bool needs_update();

                    void write_users();

                protected:
                    bool update;

                private:
                    std::list<spacepi::target::rpi::Users> system_users;
                    std::list<spacepi::target::rpi::People> human_users;

                    std::list<uid_t> uids;
                    uid_t next_uid(uid_t start);
            };
        }
    }
}

#endif
