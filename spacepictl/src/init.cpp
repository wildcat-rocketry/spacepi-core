// This is the program for initializing the spacepi
#include <iostream>

#include <spacepi/spacepictl/UserManager.hpp>
#include <spacepi/spacepictl/System.hpp>
#include <unistd.h> 
#include <sys/mount.h>

#include <boost/process.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

#define SPACEPI_ETC "/etc/spacepi"
#define FIRSTTIME_SETUP_PATH "/usr/local/sbin/install-spacepi-boardsupport"
#define NEW_CONF_PATH SPACEPI_ETC "/new_conf.xml"
#define RUNNING_CONF SPACEPI_ETC "/running_conf.xml"

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

int main(int argc, char ** argv){
    if(getpid() == 1 || argc == 2 && strcmp(argv[1], "--force-init") == 0) {
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

    fs::path setup_prog_path{FIRSTTIME_SETUP_PATH};
    if(points_to_file(setup_prog_path)){
        bp::system(setup_prog_path);
        fs::remove(FIRSTTIME_SETUP_PATH);
    }

    try{
        // Check for existence of update flag
        fs::path new_config_path{NEW_CONF_PATH};
        if(points_to_file(new_config_path)){
            run_reconfiguration();
            if(fs::exists(RUNNING_CONF)) fs::remove(RUNNING_CONF);
            fs::copy(NEW_CONF_PATH, RUNNING_CONF);

            // Delete update flag
            fs::remove(NEW_CONF_PATH);
        }
    } catch (...){
    }

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

int userspace_utility(int argc, char ** argv){
    cerr << "Userspace utility not implemented yet\n";
    return 0;
}

int run_reconfiguration(){
    ptree pt;
    
    read_xml(NEW_CONF_PATH, pt);

    optional<ptree &> options = pt.get_child_optional("config.target.options");
    if(!options){
        cerr << "config.target.options does not exist in " NEW_CONF_PATH "\n";
        return 1;
    }

    optional<ptree &> users = pt.get_child_optional("config.target.users");
    if(!users){
        cerr << "config.target.users does not exist in " NEW_CONF_PATH "\n";
        return 1;
    }

    System system(*options);
    UserManager user_man(*users);

    cout << "Done loading user changes\n";
    if(user_man.needs_update()){
        user_man.write_users();
    }
    cout << "Done writing user changes\n";

    if(system.needs_update()){
        system.write_updates();
    }
    cout << "Done writing system changes\n";

    return 0;
}
