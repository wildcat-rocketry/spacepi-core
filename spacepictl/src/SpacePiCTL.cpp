// This is the program for initializing the spacepi
#include <exception>

#include <SpacePi.hpp>
#include <spacepi/spacepictl/UserManager.hpp>
#include <spacepi/spacepictl/System.hpp>
#include <spacepi/spacepictl/SpacePiCTL.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>
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
using namespace spacepi::log;
using namespace spacepi::util;
namespace fs = boost::filesystem;
namespace bp = boost::process;

// Main
int main(int argc, char ** argv){
    SpacePiCTL control;
    Logger log("spacepictl");
    try{
        control.main(argc, argv);
    } catch (const Exception &ex){
        log(LogLevel::Error) << "Error running spacepictl: " << ex.what() << "\n" << Exception::getPointer();
    }
}

int SpacePiCTL::main(int argc, char ** argv){

    arg_count = argc;
    arg_vector = argv;

    if(getpid() == 1 || argc == 2 && strcmp(argv[1], "--force-init") == 0) {
        // Am da boss
        return initialize_system();
    }

    return userspace_utility();
}

int SpacePiCTL::initialize_system(){
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
            if(run_reconfiguration() == 0){
                FSTransaction fs;

                if(fs::exists(RUNNING_CONF)) fs.remove(RUNNING_CONF);
                fs.copy(NEW_CONF_PATH, RUNNING_CONF);

                // Delete update flag
                fs.remove(NEW_CONF_PATH);

                fs.apply();
            }
        }
    } catch (const Exception &ex){
        log(LogLevel::Error) << "Error updating system: " << ex.what() << "\n" << ex.getPointer();
    }

    // Boot into systemd
    execl("/sbin/init", "/sbin/init"); 

    return 0;
}

bool SpacePiCTL::points_to_file(fs::path& p){
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

int SpacePiCTL::userspace_utility(){
    throw EXCEPTION(StateException("User utility not implemented"));
    return 1;
}

int SpacePiCTL::run_reconfiguration(){
    ptree pt;
    FSTransaction fs_transaction;
    
    read_xml(NEW_CONF_PATH, pt);

    optional<ptree &> options = pt.get_child_optional("config.target.options");
    if(!options){
        throw EXCEPTION(StateException("config.target.options does not exist in " NEW_CONF_PATH ));
        return 1;
    }

    optional<ptree &> users = pt.get_child_optional("config.target.users");
    if(!users){
        throw EXCEPTION(StateException("config.target.users does not exist in " NEW_CONF_PATH ));
        return 1;
    }

    System system(fs_transaction, *options);
    UserManager user_man(fs_transaction, *users);

    log(LogLevel::Info) << "Done loading user changes\n";
    user_man.write_users();
    log(LogLevel::Info) << "Done writing user changes\n";

    system.write_updates();
    log(LogLevel::Info) << "Done writing system changes\n";

    try{
        log(LogLevel::Info) << "Start commiting changes\n";
        fs_transaction.apply();
        log(LogLevel::Info) << "Done commiting changes\n";
    } catch (const Exception  &ex) {
        log(LogLevel::Error) << "Error applying changes: " << ex.what() << "\n" << ex.getPointer();
        return 1;
    }

    return 0;
}
