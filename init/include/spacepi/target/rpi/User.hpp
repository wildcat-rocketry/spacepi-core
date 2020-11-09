#ifndef SPACEPI_TARGET_RPI_USER_HPP
#define SPACEPI_TARGET_RPI_USER_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <pwd.h>
#include <shadow.h>

namespace spacepi {
    namespace target {
        namespace rpi {
            class User {
                public:
                    static struct passwd* copy_passwd(struct passwd* pw);
                    static struct spwd* copy_spwd(struct spwd* sh);
                    static bool is_system_user(struct passwd* sh);
                    static bool is_system_user(User user);

                    static void destroy_passwd(struct passwd* pw);
                    static void destroy_shadow(struct spwd* sh);

                    User(const struct passwd* pw, const struct spwd* sh);
                    User(); // Start with empty struct

                    ~User(); // Free C type structs

                    std::string get_uname();

                    std::string get_pw();
                    std::string get_spw();

                    uid_t get_uid();
                    uid_t get_gid();
                    std::string get_home_dir();
                protected:
                    // Take pointers and keep them. (User now owns the pointers, don't free)
                    void add_pw(struct passwd* pw, struct spwd* sh);

                private:
                    static bool uid_system(uid_t uid);
                    static char[256] pw_buff;
                    struct passwd* passwd;
                    struct spwd* shadow;
            };
        }
    }
}

#endif
