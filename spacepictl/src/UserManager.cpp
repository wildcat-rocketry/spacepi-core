#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

#include <spacepi/spacepictl/User.hpp>
#include <spacepi/spacepictl/Person.hpp>
#include <spacepi/spacepictl/UserManager.hpp>

#include <pwd.h>
#include <grp.h>
#include <shadow.h>

using boost::property_tree::ptree;
using boost::optional;
namespace fs = boost::filesystem;
using namespace spacepi::spacepictl;
using namespace std;

UserManager::UserManager(ptree & users){
    // Iterate through users, check if system user, then add user to list
    system_users = list<User>();
    uids = list<uid_t>();

    struct passwd* cur_pwd;
    struct spwd* cur_spwd;

    while( (cur_pwd = getpwent()) ){
        if(User::uid_system(cur_pwd->pw_uid)){ 
            cur_spwd = getspnam(cur_pwd->pw_name);
            if(cur_spwd){
                system_users.push_back(User(cur_pwd, cur_spwd));
            }
        }
        uids.push_back(cur_pwd->pw_uid);
    }
    endpwent();

    human_users = list<Person>();

    cout << "Start loading users\n";
    optional<string> uname;
    optional<string> name;
    optional<string> email;
    optional<string> shell;
    optional<string> keys;

    struct group* sudo_grp = getgrnam("sudo");
    if(!sudo_grp){
        cerr << "No sudo group found\n";
        return;
    }

    gid_t sudo_gid = sudo_grp->gr_gid;

    BOOST_FOREACH(const ptree::value_type &pair, users.get_child("")){
        const ptree & user = pair.second;
        uname = user.get_optional<string>("uname");
        if(!uname){
            cerr << "User without username\n";
            continue;
        }
        name = user.get_optional<string>("name");
        email = user.get_optional<string>("email");
        shell = user.get_optional<string>("shell");
        keys = user.get_optional<string>("keys");

        cur_pwd = getpwnam((*uname).c_str());
        Person new_person = create_person(cur_pwd, cur_spwd, *uname, sudo_gid);

        new_person.add_info(name, email, shell, keys);
        human_users.push_back(new_person);
    }

    cout << "Done loading users\n";
}

Person UserManager::create_person(const struct passwd * cur_pwd, const struct spwd * cur_spwd, string uname, gid_t sudo_gid){
    if(cur_pwd){
        cur_spwd = getspnam(cur_pwd->pw_name);
        if(!cur_spwd){
            // No shadow entry somehow for user
        }
        Person new_person(cur_pwd, cur_spwd);
        human_users.push_back(new_person);
        return new_person;
    } else {
        uid_t new_uid = next_uid(1000);
        uids.push_back(new_uid);
        update = true;
        return Person(uname, new_uid, sudo_gid);
    }
}

// Recursevely find the next uid to use
uid_t UserManager::next_uid(uid_t start){
    for(uid_t const& i : uids){
        if(i == start){
            return next_uid(++start);
        }
    }
    return start;
}

bool UserManager::needs_update(){
    return this->update;
}

void UserManager::write_users(){
    ofstream pwd_f, shadow_f;
    fs::copy_file("/etc/passwd", "/etc/passwd~");
    fs::copy_file("/etc/shadow", "/etc/shadow~");
    pwd_f.open("/etc/passwd");
    shadow_f.open("/etc/shadow");

    for(User const& i : system_users){
        pwd_f << i.get_pw() << endl;
        shadow_f << i.get_spw() << endl;
    }

    for(Person const& i : human_users){
        pwd_f << i.get_pw() << endl;
        shadow_f << i.get_spw() << endl;
    }

    pwd_f.close();
    shadow_f.close();
}
