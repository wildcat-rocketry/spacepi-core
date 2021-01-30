#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <cstring>
#include <spacepi/spacepictl/User.hpp>

#include <sstream>

#include <pwd.h>
#include <shadow.h>

using namespace std;
using namespace spacepi::spacepictl;

// Constructors
User::User(const struct passwd* pw, const struct spwd* sh) :
    pw_uid(pw->pw_uid), pw_gid(pw->pw_gid), pw_name(pw->pw_name), pw_passwd(pw->pw_passwd),
    pw_gecos(pw->pw_gecos), pw_dir(pw->pw_dir), pw_shell(pw->pw_shell),
    sp_lstchg(sh->sp_lstchg), sp_min(sh->sp_min), sp_max(sh->sp_max), sp_warn(sh->sp_warn),
    sp_inact(sh->sp_inact), sp_expire(sh->sp_expire), sp_pwdp(sh->sp_pwdp) {
}

User::User(string uname, uid_t uid, gid_t gid) :
    pw_name(uname), pw_uid(uid), pw_gid(gid), pw_dir("/home/" + uname) {
}

// Getters

string User::get_uname() const {
    return pw_name;
}

uid_t User::get_uid() const {
    return pw_uid;
}

gid_t User::get_gid() const {
    return pw_gid;
}

string User::get_home_dir() const {
    return pw_dir;
}

// Get the string for this user for /etc/passwd
string User::get_pw() const {
    string pwd;
    stringstream ss;

    ss << pw_name << ":";
    ss << pw_passwd << ":";
    ss << negative_blank(pw_uid) << ":";
    ss << negative_blank(pw_gid) << ":";
    ss << pw_gecos << ":";
    ss << pw_dir << ":";
    ss << pw_shell;
    pwd = ss.str();

    cout << "Passwd line for " << get_uname() << ": " << pwd << "\n";
    return pwd;
}

// Get the string for this user for /etc/shadow
string User::get_spw() const {
    string spw;
    stringstream ss;
    ss << pw_name << ":"; // sp_namp -> pw_name
    ss << sp_pwdp << ":";
    ss << negative_blank(sp_lstchg) << ":";
    ss << negative_blank(sp_min) << ":";
    ss << negative_blank(sp_max) << ":";
    ss << negative_blank(sp_warn) << ":";
    ss << negative_blank(sp_inact) << ":";
    ss << negative_blank(sp_expire) << ":"; // Trailing : for flag
    spw = ss.str();

    cout << "Shadow line for " << get_uname() << ": " << spw << "\n";
    return spw;
}

// Convert the int to a string, but the string is blank if the int is negative
// Usefull for /etc/passwd and /etc/shadow strings
string User::negative_blank(long int num){
    if(num >= 0){
        return to_string(num);
    } else {
        return "";
    }
}

// Is this user a system user !(1000 <= uid < 10000)
bool User::is_system_user(const struct passwd* pw){
    return is_system_user(pw->pw_uid);
}

// Is this user a system user !(1000 <= uid < 10000)
bool User::is_system_user(User user){
    return is_system_user(user.get_uid());
}

// Is this user a system user !(1000 <= uid < 10000)
bool User::is_system_user(uid_t uid){
    return uid < 1000 || uid >= 10000;
}
