#ifndef SPACEPI_TARGET_RPI_PERSON_HPP
#define SPACEPI_TARGET_RPI_PERSON_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/User.hpp>
#include <pwd.h>
#include <shadow.h>

namespace spacepi {
    namespace spacepictl {
        class Person: public spacepi::spacepictl::User , private spacepi::log::AutoLog<decltype("spacepictl:Person"_autolog)> {
            public:
                // Makes a new person with defaults
                Person(std::string uname, uid_t uid, gid_t gid);

                // Make a new person by copying data from C library
                Person(const struct passwd* pw, const struct spwd* sh);


                void add_info(boost::optional<std::string> new_name, boost::optional<std::string> new_email, boost::optional<std::string> new_shell, boost::optional<std::string> new_keys);

                void update_user();

                static char *set_string(std::string to_set);

            protected:
                void update_info(boost::optional<std::string> &old_param, boost::optional<std::string> &new_param, bool &update_flag);

                // Build a new default home dir for the listed user
                static void build_home(std::string uname, uid_t uid, gid_t gid); 

                void write_keys();
                void update_git();

                boost::optional<std::string> name;
                boost::optional<std::string> email;
                boost::optional<std::string> shell;
                boost::optional<std::string> keys;

                bool update_name;
                bool update_email;
                bool update_shell;
                bool update_keys;

            private:
        };
    }
}

#endif
