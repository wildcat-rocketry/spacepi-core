#ifndef SPACEPI_TARGET_RPI_USER_HPP
#define SPACEPI_TARGET_RPI_USER_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <pwd.h>
#include <shadow.h>

namespace spacepi {
    namespace target {
        namespace rpi {
            class User {
                public:
                    static struct passwd* copy_passwd(const struct passwd* pw);
                    static struct spwd* copy_spwd(const struct spwd* sh);
                    static bool is_system_user(const struct passwd* sh);
                    static bool is_system_user(User user);

                    static void destroy_passwd(struct passwd* pw);
                    static void destroy_shadow(struct spwd* sh);

                    User(const struct passwd* pw, const struct spwd* sh);
                    User(); // Start with empty struct

                    ~User(); // Free C type structs

                    std::string get_uname();

                    std::string get_pw() const;
                    std::string get_spw() const;

                    uid_t get_uid();
                    uid_t get_gid();
                    std::string get_home_dir();

                    static bool uid_system(uid_t uid);

                protected:
                    // Take pointers and keep them. (User now owns the pointers, don't free)
                    void add_pw(struct passwd* pw, struct spwd* sh);

                private:
                    static std::string negative_blank(int num);
                    static char pw_buff[256];
                    struct passwd* passwd;
                    struct spwd* shadow;
            };
        }
    }
}

#endif
