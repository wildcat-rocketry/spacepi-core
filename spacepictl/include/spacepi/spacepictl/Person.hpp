#ifndef SPACEPI_TARGET_RPI_PERSON_HPP
#define SPACEPI_TARGET_RPI_PERSON_HPP

#include <string>
#include <vector>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>
#include <spacepi/spacepictl/User.hpp>
#include <pwd.h>
#include <shadow.h>

namespace spacepi {
    namespace spacepictl {
        class Person: public spacepi::spacepictl::User , private spacepi::log::AutoLog<decltype("spacepictl:Person"_autolog)> {
            public:
                // Makes a new person with defaults
                Person(spacepi::spacepictl::FSTransaction &fs, std::string uname, uid_t uid, gid_t gid);

                // Make a new person by copying data from C library
                Person(spacepi::spacepictl::FSTransaction &fs, const struct passwd* pw, const struct spwd* sh);


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
                spacepi::spacepictl::FSTransaction &fs;
        };
    }
}

#endif
