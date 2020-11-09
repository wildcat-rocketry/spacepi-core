#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <spacepi/target/rpi/User>
#include <spacepi/util/CommandConfigurable>

#include <unistd.h>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/process.hpp>

#include <pwd.h>
#include <shadow.h>

using namespace std;
using namespace spacepi::target::rpi;
using namespace spacepi::util;
namespace fs = boost::filesystem;
namespace bp = boost::process;

Person::Person(const struct passwd* pw, const struct spwd* sh) : User(pw, sh) {
}

Person::Person(string uname, uid_t uid, gid_t gid){
    // Create the default person info and build the default home
    struct passwd* pw = new struct passwd;
    struct spwd* sh = new struct spwd;
    make_defualt_pw(pw, sh, uname, uid, gid);
    this.add_pw(pw, sh);
    
    build_home(uname, uid, gid);
}

static void Person::make_default_pw(pw, sh, uname, uid, gid){
    pw->pw_uid = uid;
    pw->pw_gid = gid;
    pw->pw_passwd = set_string("x");
    pw->pw_gecos = set_string("From spacepi init");
    pw->pw_name = set_string(uname);
    pw->pw_dir = set_string("/home/" / uname);
    pw->pw_shell = set_string("/bin/bash");

    sh->sp_namp = set_string(uname);
    sh->sp_pwdp = set_string("*");
    sh->sp_lstchg = 18509;
    sh->sp_min = 0;
    sh->sp_max = 99999;
    sh->sp_warn = 7;
    sh->sp_inact = -1;
    sh->sp_expire = -1;
    sh->sp_flag = -1;
}

void Person::update_info(optional<string> &old_param, optional<string> &new_param, bool &update_flag){
    if(new_param){
        if(old_param){
            if((*old_param).compare(*new_param)){
                update_flag = 1;
                old_param = new_param;
            } else {
                update_flag = 0
            }
        } else {
            old_param = new_param;
            update_flag = 1;
        }
    } else {
        update_flag = 0;
    }
}

void Person::add_info(optional<string> new_name, optional<string> new_email, optional<string> new_shell, optional<string> new_keys){
    update_info(name, new_name, update_name);    
    update_info(email, new_email, update_email);    
    update_info(shell, new_shell, update_shell);    
    update_info(keys, new_keys, update_keys);    
}

static char * Person::set_string(string to_set){
    char * new_array = new char[to_set.length() + 1];
    to_set.copy(new_array, to_set.length());
    return new_array;
}

static void build_home(string uname, uid_t uid, gid_t gid){
    fs::path home_dir_name("/home/" + uname); 

    if(!fs::exists(home_dir_name)){
        create_directory(home_dir_name);
    }
    chown(home_dir_name.c_str(), uid, gid);

    // Loop through skell dir and 
    for(auto& entry : boost::make_iterator_range(fs::directory_iterator("/etc/skel"), {})){
        path old_file_path = entry.path();
        path new_file_path = home_dir_name + "/" + old_file_path.filename();
        if(fs::is_regular_file(old_file_path)){
            if(!fs::exists(new_file_path)){
                fs::copy_file(old_file_path, new_file_path);
            }
            chown(new_file_path.c_str(), uid, gid);
        }
    }

    // This is where I can set up default symlink to github ssh keys
}

void Person::write_keys(){
    if(keys){
        ofstream file;
        path key_file = this.get_home_dir() + "/.ssh/authorized_keys";
        file.open(key_file);
        file << keys << endl;
        file.close();
        chown(key_file, this.get_uid(), this.get_gid());
    }
}

void person::update_git(){
    // Become user and update GIT
    int pid = fork();
    if(pid == 0){ // Am child
        // Become child
        setuid(this.get_uid());
        setgid(this.get_gid());

        setenv("HOME", this.get_home_dir(), 1); // Set home to child home

        if(name){
            bp::system("git config --global user.name " + *name);
        }

        if(email){
            bp::system("git config --global user.email " + *email);
        }

        exit(0);

    } else if (pid == -1) { // Is error
        cerr << "Error forking to update user: " << strerror(errno) << "\n";
        return 1;
    } else { // Is parent
        wait(NULL);
    }

}

void person::update_user(){
    if(update_keys){
        this.write_keys();
    }

    if(update_name || update_email){
        this.update_git();
    }
}
