// This is the program for initializing the spacepi
#include <exception>
#include <vector>

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
#include <boost/foreach.hpp>

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
int main(int argc, char* argv[]){
    SpacePiCTL control;
    Logger log("spacepictl");
    try{
        return control.main(vector<string>(&argv[0], &argv[argc]));
    } catch (const Exception &ex){
        log(LogLevel::Error) << "Error running spacepictl: " << ex.what() << "\n" << ex.getPointer();
    }

    return 0;
}

int SpacePiCTL::main(vector<string> argv){

    if(getpid() == 1 || argv.size() == 2 && argv[1] == "--force-init") {
        // Am da boss
        return initialize_system();
    }

    return userspace_utility(argv);
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

    log(LogLevel::Info) << "Starting /sbin/init";
    // Boot into systemd
    execl("/sbin/init", "/sbin/init", nullptr); 
    log(LogLevel::Error) << "Error executing /sbin/init: " << strerror(errno) << "!";
    return 1;
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

int SpacePiCTL::userspace_utility(vector<string> argv){
    if(argv.size() > 1){
        try{
            if(argv[1] == "exec"){
                return spacepictl_exec(argv);
            } else if(argv[1] == "list") {
                return spacepictl_list(argv);
            } else {
                cerr << "No action \"" << argv[1] << "\"\n\n";
            }
        } catch (const Exception &ex){
            log(LogLevel::Error) << "Error running spacepictl " << argv[1] << ": " << ex.what() << "\n" << ex.getPointer();
        }

    } else {
        cerr << "Not enough arguments.\n\n";
    }

    cerr << "USAGE: " << argv[0] << " <action> [[options] ... ]\n";
    cerr << "Available actions:\n";
    cerr << "    exec     Execute a SpacePi service\n";
    cerr << "    list     List SpacePi services\n";
    return 1;
}

bool SpacePiCTL::get_module(string module_name, spacepi::package::Module &module_config){
    unordered_map<string, spacepi::package::Module> modules = (spacepi::package::PackageConfig(RUNNING_CONF)).getModules();


    for( const auto &module_pair : modules){
        spacepi::package::Module module = module_pair.second;
        if(module.getName() == module_name){
            module_config = module;
            return true;
        }
    }

    return false;
}

int SpacePiCTL::spacepictl_exec(vector<string> argv){
    if(argv.size() == 3){
        spacepi::package::Module module;
        if(get_module(argv[2], module)){
            cout << "Found module \"" << argv[2] << "\"\n";
            vector<char*> args;
            vector<string> str_args;
            string executable = System::moduleBin(module);
            args.push_back(const_cast<char*>(executable.c_str()));

            spacepi::package::Options options = module.getOptions();
            for( const auto& option : options){
                str_args.push_back("--" + option.getKey());
                str_args.push_back(option.getValue());
            }

            for(const auto &arg : str_args){
                args.push_back(const_cast<char*>(arg.c_str()));
            }
            args.push_back(NULL);

            execvp(executable.c_str(), &args[0]);

            stringstream ss;
            ss << "Error executing";
            for(auto &arg : args) ss << " " << arg;
            log(LogLevel::Error) << ss.str();

            return 1;
        } else {
            cerr << "Module \"" << argv[2] << "\" not found\n";
        }
    } else {
        cerr << "Incorrect number of arguments (" << argv.size() << "). Expects 3.\n\n";
    }

    cerr << "USAGE: " << argv[0] << " " << argv[1] << " UNIT_NAME\n";
    return 1;
}

// List the modules in the config by name
int SpacePiCTL::spacepictl_list(vector<string> argv){
    if(argv.size() == 2){
        unordered_map<string, spacepi::package::Module> modules = (spacepi::package::PackageConfig(RUNNING_CONF)).getModules();
        for(const auto& module_pair : modules){
            cout << module_pair.second.getName() << endl;
        }
        return 0;
    } else {
        cerr << "Incorrect number of arguments (" << argv.size() << "). Expects 2.\n\n";
    }

    cerr << "USAGE: " << argv[0] << " " << argv[1] << "\n";
    return 1;
}

int SpacePiCTL::run_reconfiguration(){
    ptree pt;
    FSTransaction fs_transaction;
    
    read_xml(NEW_CONF_PATH, pt);

    spacepi::package::PackageConfig config(NEW_CONF_PATH);

    System system(fs_transaction, config.getTargetOptions(), config.getModules());
    UserManager user_man(fs_transaction, config.getUsers());

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
