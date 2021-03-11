#include <string>
#include <map>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/FSTransaction.hpp>
#include <spacepi/spacepictl/util/FSOStream.hpp>
#include <spacepi/spacepictl/util/User.hpp>
#include <spacepi/spacepictl/util/Person.hpp>

#include <unistd.h>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/process.hpp>

#include <pwd.h>
#include <shadow.h>

using namespace std;
using namespace spacepi::spacepictl::util;
using namespace spacepi::util;
namespace fs = boost::filesystem;
namespace bp = boost::process;

using boost::optional;

Person::Person(FSTransaction &fs, const struct passwd* pw, const struct spwd* sh) : User(pw, sh), fs(fs) {
}

Person::Person(FSTransaction &fs, string uname, uid_t uid, gid_t gid) : User(uname, uid, gid), fs(fs){
    build_home(uname, uid, gid);
}

void Person::add_info(const std::string &new_name, const std::string &new_email, std::vector<std::string> new_keys){
    name = new_name;
    email = new_email;
    keys = new_keys;
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
    string key_file = this->get_home_dir() + "/.ssh/authorized_keys";
    if(!fs::exists(get_home_dir() + "/.ssh")){
        fs.mkdir(get_home_dir() + "/.ssh");
    }

    FSOStream file(fs, key_file, get_uid(), get_gid());
    for(const auto& key : keys){
        file << key << endl;
    }
}

void Person::update_git(){
    map<string,string> confEntries;

    if(name != ""){
        confEntries.insert({"user.name", name});
    }

    if(email != ""){
        confEntries.insert({"user.email", email});
    }

    if(confEntries.size() > 0){
        fs.add_git_config(get_home_dir() + "/.gitconfig", confEntries, get_uid(), get_gid());
    }
}

void Person::update_user(){
    this->write_keys();
    this->update_git();
}
