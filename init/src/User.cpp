#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <spacepi/target/rpi/User>
#include <spacepi/util/CommandConfigurable>

#include <pwd.h>
#include <shadow.h>

using namespace std;
using namespace spacepi::target::rpi;
using namespace spacepi::util;

User::User(const struct passwd* pw, const struct spwd* sh) {
    passwd = User::copy_passwd(pw);
    shadow = User::copy_shadow(spwd);
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

static void User::destroy_passwd(struct passwd* pw){
    if(pw){
        delete [] pw->pw_name;
        delete [] pw->pw_passwd;
        delete [] pw->pw_gecos;
        delete [] pw->pw_dir;
        delete [] pw->pw_shell;
        delete pw;
    }
}

static void User::destroy_shadow(struct spwd* sh){
    if(sh){
        delete [] sh->sp_namp;
        delete [] sh->sp_pwdp;
        delete sh;
    }
}

string User::get_uname(){
    string uname;
    if(passwd){
        uname = new string(passwd->pw_name);
    } else {
        uname = new string("");
    }
    return uname;
}

string User::get_pw(){
    string pwd;
    if(passwd){
        
    } else {
        pwd = string("");
    }
    return pwd;
}

static struct passwd* User::copy_passwd(const struct passwd* pw){
    struct passwd* clone = new struct passwd
    clone->pw_uid = old->pw_uid;
    clone->pw_gid = old->pw_gid;
    clone->pw_name = new char[strlen(old->pw_name) + 1];
    clone->pw_passwd = new char[strlen(old->pw_passwd) + 1];
    clone->pw_gecos = new char[strlen(old->pw_gecos) + 1];
    clone->pw_dir = new char[strlen(old->pw_dir) + 1];
    clone->pw_shell = new char[strlen(old->pw_shell) + 1];

    strcpy(clone->pw_name, old->pw_name);
    strcpy(clone->pw_passwd, old->pw_passwd);
    strcpy(clone->pw_gecos, old->pw_gecos);
    strcpy(clone->pw_dir, old->pw_dir);
    strcpy(clone->pw_shell, old->pw_shell);

    return clone;
}

static struct spwd* User::copy_spwd(const struct spwd* sh){
    struct spwd* clone = new struct spwd;
    clone->sp_lstchg = old->sp_lstchg;
    clone->sp_min = old->sp_min;
    clone->sp_max = old->sp_max;
    clone->sp_warn = old->sp_warn;
    clone->sp_inact = old->sp_inact;
    clone->sp_expire = old->sp_expire;
    clone->sp_namp = new char[strlen(old->sp_namp) + 1];
    clone->sp_pwdp = new char[strlen(old->sp_pwdp) + 1];

    strcpy(clone->sp_namp, old->sp_namp);
    strcpy(clone->sp_pwdp, old->sp_pwdp);

    return clone;
}

static bool User::is_system_user(struct passwd* pw){
    return User::uid_system(pw->pw_uid);
}

static bool is_system_user(User user){
    return User::uid_system(user.get_uid());
}

static bool User::uid_system(uid_t uid){
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
    return string(pw->pw_dir);
}
