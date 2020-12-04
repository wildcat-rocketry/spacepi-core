#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <spacepi/target/rpi/User.hpp>

#include <sstream>

#include <pwd.h>
#include <shadow.h>

using namespace std;
using namespace spacepi::target::rpi;

User::User(const struct passwd* pw, const struct spwd* sh) {
    passwd = copy_passwd(pw);
    shadow = copy_spwd(sh);
}

User::User(){
    // Start with empty structs
    passwd = new struct passwd;
    shadow = new struct spwd;
}

User::~User(){
    // Free C style structs
    if(passwd){
        destroy_passwd(passwd);
        passwd = NULL;
    }

    if(shadow){
        destroy_shadow(shadow);
        shadow = NULL;
    }
}

void User::destroy_passwd(struct passwd* pw){
    if(pw){
        delete [] pw->pw_name;
        delete [] pw->pw_passwd;
        delete [] pw->pw_gecos;
        delete [] pw->pw_dir;
        delete [] pw->pw_shell;
        delete pw;
    }
}

void User::destroy_shadow(struct spwd* sh){
    if(sh){
        delete [] sh->sp_namp;
        delete [] sh->sp_pwdp;
        delete sh;
    }
}

string User::get_uname(){
    string uname;
    if(passwd){
        uname = string(passwd->pw_name);
    } else {
        uname = string("");
    }
    return uname;
}

string User::get_pw() const {
    string pwd;
    if(passwd){
        stringstream ss;
        ss << string(passwd->pw_name) << ":";
        ss << string(passwd->pw_passwd) << ":";
        ss << negative_blank(passwd->pw_uid) << ":";
        ss << negative_blank(passwd->pw_gid) << ":";
        ss << string(passwd->pw_gecos) << ":";
        ss << string(passwd->pw_dir) << ":";
        ss << string(passwd->pw_shell);
        ss >> pwd; // Stream back into pwd

    } else {
        pwd = string("");
    }
    return pwd;
}

string User::get_spw() const {
    string spw;
    if(shadow){
        stringstream ss;
        ss << string(shadow->sp_namp) << ":";
        ss << string(shadow->sp_pwdp) << ":";
        ss << negative_blank(shadow->sp_lstchg) << ":";
        ss << negative_blank(shadow->sp_min) << ":";
        ss << negative_blank(shadow->sp_max) << ":";
        ss << negative_blank(shadow->sp_warn) << ":";
        ss << negative_blank(shadow->sp_inact) << ":";
        ss << negative_blank(shadow->sp_expire);
        ss >> spw; // Stream back into pwd
    } else {
        spw = string("");
    }
    return spw;
}

string User::negative_blank(int num){
    if(num >= 0){
        return to_string(num);
    } else {
        return "";
    }
}

struct passwd* User::copy_passwd(const struct passwd* pw){
    struct passwd* clone = new struct passwd;
    clone->pw_uid = pw->pw_uid;
    clone->pw_gid = pw->pw_gid;
    clone->pw_name = new char[strlen(pw->pw_name) + 1];
    clone->pw_passwd = new char[strlen(pw->pw_passwd) + 1];
    clone->pw_gecos = new char[strlen(pw->pw_gecos) + 1];
    clone->pw_dir = new char[strlen(pw->pw_dir) + 1];
    clone->pw_shell = new char[strlen(pw->pw_shell) + 1];

    strcpy(clone->pw_name, pw->pw_name);
    strcpy(clone->pw_passwd, pw->pw_passwd);
    strcpy(clone->pw_gecos, pw->pw_gecos);
    strcpy(clone->pw_dir, pw->pw_dir);
    strcpy(clone->pw_shell, pw->pw_shell);

    return clone;
}

struct spwd* User::copy_spwd(const struct spwd* sh){
    struct spwd* clone = new struct spwd;
    clone->sp_lstchg = sh->sp_lstchg;
    clone->sp_min = sh->sp_min;
    clone->sp_max = sh->sp_max;
    clone->sp_warn = sh->sp_warn;
    clone->sp_inact = sh->sp_inact;
    clone->sp_expire = sh->sp_expire;
    clone->sp_namp = new char[strlen(sh->sp_namp) + 1];
    clone->sp_pwdp = new char[strlen(sh->sp_pwdp) + 1];

    strcpy(clone->sp_namp, sh->sp_namp);
    strcpy(clone->sp_pwdp, sh->sp_pwdp);

    return clone;
}

bool User::is_system_user(const struct passwd* pw){
    return User::uid_system(pw->pw_uid);
}

bool User::is_system_user(User user){
    return User::uid_system(user.get_uid());
}

bool User::uid_system(uid_t uid){
    return uid < 1000 && uid > 10000;
}

uid_t User::get_uid(){
    return passwd->pw_uid;
}

gid_t User::get_gid(){
    return passwd->pw_gid;
}

void User::add_pw(struct passwd* pw, struct spwd* sh){
    passwd = pw;
    shadow = sh;
}

string User::get_home_dir(){
    return string(passwd->pw_dir);
}
