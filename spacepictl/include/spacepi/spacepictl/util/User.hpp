#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_UTIL_USER_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_UTIL_USER_HPP

#include <SpacePi.hpp>
#include <string>
#include <memory>
#include <vector>
#include <boost/program_options.hpp>
#include <pwd.h>
#include <shadow.h>

namespace spacepi {
    namespace spacepictl {
        namespace util {
            class User: protected spacepi::log::AutoLog<decltype("spacepictl:user"_autolog)> {
                public:
                    static bool is_system_user(const struct passwd* sh);
                    static bool is_system_user(User user);
                    static bool is_system_user(uid_t uid);

                    User(const struct passwd* pw, const struct spwd* sh);
                    User(std::string uname, uid_t uid, gid_t gid);

                    std::string get_uname() const;

                    std::string get_pw() const;
                    std::string get_spw() const;

                    uid_t get_uid() const;
                    uid_t get_gid() const;
                    std::string get_home_dir() const;

                private:
                    static std::string negative_blank(long int num);

                    // Members of struct passwd
                    const uid_t pw_uid;
                    const gid_t pw_gid;
                    const std::string pw_passwd = "x";
                    const std::string pw_name;
                    const std::string pw_gecos = "From spacepictl";
                    const std::string pw_dir;
                    const std::string pw_shell = "/bin/bash";
                    
                    // Members of struct spwd
                    // sp_namp -> pw_name
                    const std::string sp_pwdp = "";
                    const long int sp_lstchg = 18509;
                    const long int sp_min = 0;
                    const long int sp_max = 99999;
                    const long int sp_warn = 7;
                    const long int sp_inact = -1;
                    const long int sp_expire = -1;
            };
        }
    }
}

#endif
