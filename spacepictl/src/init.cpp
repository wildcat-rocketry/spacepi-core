// This is the program for initializing the spacepi
#include <iostream>

#include <spacepi/target/rpi/UserManager.hpp>
#include <spacepi/target/rpi/System.hpp>
#include <unistd.h> 

#include <boost/property_tree/xml_parser.hpp>

#define CONFIG_PATH "/etc/spacepi.xml"

using namespace std;

using boost::property_tree::ptree;
using boost::optional;
using namespace spacepi::target::rpi;

int main(int argc, char ** argv){
    if(getpid() == 1) {
        // Am da boss
        return initialize_system();
    }

    if(geteuid() != 0){
        cerr << "Program should be run as root\n";
    }

    return userspace_utility(argc, argv);
}

int initialize_system(){
    // Check for existence of setup program link
    //string setup_prog_path = "/etc/spacepi/setup"

    // Delete setup program link if setup doesn't fail

    // Check for existence of update flag
    // Delete update flag

    // Boot into systemd
    execl("/sbin/init", ""); 
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
