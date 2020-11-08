#ifndef SPACEPI_TARGET_RPI_PERSON_HPP
#define SPACEPI_TARGET_RPI_PERSON_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/target/rpi/User>

namespace spacepi {
    namespace target {
        namespace rpi {
            class Person (spacepi::target::rpi::User) {
                public:
                    static char * set_string(string to_set);
                    Person(string uname, uid_t uid, gid_t gid); // Makes a new person with defaults

                    // Make a new person by copying data from C library
                    Person(struct passwd* pw, struct spwd* sh);

                    void add_info(optional<string> new_name, optional<string> new_email, optional<string> new_shell, optional<string> new_keys);
                    void update_user();
                    void write_user();

                protected:
                    void update_info(optional<string> &old_param, optional<string> &new_param, bool &update_flag);
                    static void make_defualt_pw(struct passwd* pw, struct spwd* sh, string uname, uid_t uid);

                    // Build a new default home dir for the listed user
                    static void build_home(string uname, uid_t uid, gid_t gid); 

                    optional<string> name;
                    optional<string> email;
                    optional<string> shell;
                    optional<string> keys;

                    bool update_name;
                    bool update_email;
                    bool update_shell;
                    bool update_keys;

                private:
            };
        }
    }
}

#endif
