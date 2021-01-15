// This is the program for initializing the spacepi
#include <iostream>

#include <spacepi/spacepictl/UserManager.hpp>
#include <spacepi/spacepictl/System.hpp>
#include <unistd.h> 
#include <sys/mount.h>

#include <boost/process.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

#define CONFIG_PATH "/etc/spacepi.xml"

using namespace std;

using boost::property_tree::ptree;
using boost::optional;
using namespace spacepi::spacepictl;
namespace fs = boost::filesystem;
namespace bp = boost::process;

int initialize_system();
int userspace_utility(int argc, char ** argv);
int run_reconfiguration();
bool points_to_file(fs::path& p);
bool is_readonlyfs(string test_file_path = "/rwtest");
void ensure_root_rw();

int main(int argc, char ** argv){
    if(getpid() == 1) {
        // Am da boss
        return initialize_system();
    }

    if(geteuid() != 0){
        cerr << "Program should be run as root\n";
        return 1;
    }

    return userspace_utility(argc, argv);
}

int initialize_system(){
    // Check for existence of setup program link

    bool is_ro = is_readonlyfs();

    fs::path setup_prog_path{"/etc/spacepi/setup"};
    if(points_to_file(setup_prog_path)){
        bp::system(setup_prog_path);
        if(is_ro) mount(NULL, "/", NULL, MS_REMOUNT, NULL);
        fs::remove("/etc/spacepi/setup");
    }

    // Delete setup program link if setup doesn't fail

    // Check for existence of update flag
    // Delete update flag

    if(is_ro) mount(NULL, "/", NULL, MS_REMOUNT | MS_RDONLY, NULL);
    // Boot into systemd
    execl("/sbin/init", "/sbin/init"); 
    return 0;
}

bool points_to_file(fs::path& p){
    fs::path canon;
    if(fs::exists(p)){
        canon = fs::canonical(p);
        if(fs::is_regular_file(canon)){
            p = canon;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool is_readonlyfs(string test_file_path){
    ofstream testFile (test_file_path);
    if(!testFile.is_open()){
        return true;
    }

    testFile.close();

    return false;
}

int userspace_utility(int argc, char ** argv){
    cerr << "Userspace utility not implemented yet\n";
    return 0;
}

int run_reconfiguration(){
    ptree pt;
    
    read_xml(CONFIG_PATH, pt);

    optional<ptree &> options = pt.get_child_optional("config.target.options");
    if(!options){
        cerr << "config.target.options does not exist in " CONFIG_PATH "\n";
        return 1;
    }

    optional<ptree &> users = (*options).get_child_optional("users");
    if(!options){
        cerr << "config.target.options does not exist in " CONFIG_PATH "\n";
        return 1;
    }

    if(!users){
        cerr << "config.target.options.users does not exist in " CONFIG_PATH "\n";
        return 1;
    }

    System system(*options);
    UserManager user_man(*users);

    if(user_man.needs_update()){
        user_man.write_users();
    }

    if(system.needs_update()){
        system.write_updates();
    }

    return 0;
}
