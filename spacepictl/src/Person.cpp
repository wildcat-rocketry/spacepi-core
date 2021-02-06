#include <string>
#include <map>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>
#include <spacepi/spacepictl/FSOStream.hpp>
#include <spacepi/spacepictl/User.hpp>
#include <spacepi/spacepictl/Person.hpp>

#include <unistd.h>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/process.hpp>
#include <boost/optional.hpp>

#include <pwd.h>
#include <shadow.h>

using namespace std;
using namespace spacepi::spacepictl;
using namespace spacepi::util;
namespace fs = boost::filesystem;
namespace bp = boost::process;

using boost::optional;

Person::Person(FSTransaction &fs, const struct passwd* pw, const struct spwd* sh) : User(pw, sh), fs(fs) {
}

Person::Person(FSTransaction &fs, string uname, uid_t uid, gid_t gid) : User(uname, uid, gid), fs(fs){
    build_home(uname, uid, gid);
}

void Person::update_info(optional<string> &old_param, optional<string> &new_param, bool &update_flag){
    if(new_param){
        if(old_param){
            if(*old_param != *new_param){
                update_flag = 1;
                old_param = new_param;
            } else {
                update_flag = 0;
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

char * Person::set_string(string to_set){
    char * new_array = new char[to_set.length() + 1];
    to_set.copy(new_array, to_set.length());
    new_array[to_set.length()] = '\0';
    return new_array;
}

void Person::build_home(string uname, uid_t uid, gid_t gid){
    fs::path home_dir_name("/home/" + uname); 

    if(!fs::exists(home_dir_name)){
        fs.mkdir(home_dir_name.native(), uid, gid);
    }

    // Loop through skell dir and 
    for(auto& entry : boost::make_iterator_range(fs::directory_iterator("/etc/skel"), {})){
        fs::path old_file_path = entry.path();
        fs::path new_file_path = home_dir_name.native() / fs::path("/") / old_file_path.filename();
        if(fs::is_regular_file(old_file_path)){
            if(!fs::exists(new_file_path)){
                fs.copy(old_file_path.native(), new_file_path.native(), uid, gid);
            }
        }
    }

    // This is where I can set up default symlink to github ssh keys
}

void Person::write_keys(){
    if(keys){
        string key_file = this->get_home_dir() + "/.ssh/authorized_keys";
        if(!fs::exists(get_home_dir() + "/.ssh")){
            fs.mkdir(get_home_dir() + "/.ssh");
        }

        FSOStream file(fs, key_file, get_uid(), get_gid());
        file << *keys << endl;
    }
}

void Person::update_git(){
    map<string,string> confEntries;

    if(name){
        confEntries.insert({"user.name", *name});
    }

    if(email){
        confEntries.insert({"user.email", *email});
    }

    if(confEntries.size() > 0){
        fs.add_git_config(get_home_dir() + "/.gitconfig", confEntries, get_uid(), get_gid());
    }
}

void Person::update_user(){
    if(update_keys){
        this->write_keys();
    }

    if(update_name || update_email){
        this->update_git();
    }
}
