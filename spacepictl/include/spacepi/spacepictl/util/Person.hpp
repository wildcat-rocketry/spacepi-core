#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_UTIL_PERSON_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_UTIL_PERSON_HPP

#include <string>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/FSTransaction.hpp>
#include <spacepi/spacepictl/util/User.hpp>
#include <pwd.h>
#include <shadow.h>

namespace spacepi {
    namespace spacepictl {
        namespace util {
            class Person: public spacepi::spacepictl::util::User , private spacepi::log::AutoLog<decltype("spacepictl:person"_autolog)> {
                public:
                    // Makes a new person with defaults
                    Person(spacepi::spacepictl::util::FSTransaction &fs, std::string uname, uid_t uid, gid_t gid);

                    // Make a new person by copying data from C library
                    Person(spacepi::spacepictl::util::FSTransaction &fs, const struct passwd* pw, const struct spwd* sh);


                    void add_info(const std::string &new_name, const std::string &new_email, std::vector<std::string> new_keys);

                    void update_user();
                protected:
                    // Build a new default home dir for the listed user
                    void build_home(std::string uname, uid_t uid, gid_t gid); 

                    void write_keys();
                    void update_git();

                    std::string name;
                    std::string email;
                    std::vector<std::string> keys;

                private:
                    spacepi::spacepictl::util::FSTransaction &fs;
            };
        }
    }
}

#endif
