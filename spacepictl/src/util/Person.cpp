#include <string>
#include <map>
#include <stack>
#include <utility>
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
using namespace spacepi::log;
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
    stack<pair<fs::path, fs::path>> dirStack;
    dirStack.emplace("/home" / fs::path(uname), fs::path("/etc/skel"));

    while (!dirStack.empty()) {
        fs::path dst = dirStack.top().first;
        fs::path src = dirStack.top().second;
        dirStack.pop();

        if (!fs::exists(dst)) {
            fs.mkdir(dst.native(), uid, gid);
        }

        for (fs::directory_iterator it(src); it != fs::directory_iterator(); ++it) {
            fs::path name = it->path().filename();
            switch (it->status().type()) {
                case fs::file_type::directory_file:
                    dirStack.emplace(dst / name, src / name);
                    break;
                case fs::file_type::file_not_found:
                case fs::file_type::regular_file:
                case fs::file_type::symlink_file:
                    if (!fs::exists(dst / name)) {
                        if (fs::is_symlink(src / name)) {
                            fs.link((dst / name).native(), fs::read_symlink(src / name).native(), uid, gid);
                        } else {
                            fs.copy((src / name).native(), (dst / name).native(), uid, gid);
                        }
                    }
                    break;
                default:
                    log(LogLevel::Warning) << "Skipping skel file due to invalid type: " << it->path() << "(" << it->status().type() << ")";
                    break;
            }
        }
    }
}

void Person::write_keys(){
    string key_file = this->get_home_dir() + "/.ssh/authorized_keys";
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
