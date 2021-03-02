// This is the program for initializing the spacepi
#include <exception>
#include <vector>
#include <algorithm>

#include <SpacePi.hpp>
#include <spacepi/spacepictl/Config.hpp>
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
#define SPACEPI_CONFIGS "/usr/local/src/spacepi/configs"
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
            } else if(argv[1] == "start" || argv[1] == "stop" || argv[1] == "status" || argv[1] == "enable" ||
                      argv[1] == "disable" || argv[1] == "restart" || argv[1] == "reenable") {
                return spacepictl_systemctl(argv);
            } else if(argv[1] == "config-set") {
                return spacepictl_config_set(argv);
            } else {
                log(LogLevel::Error) << "No action \"" << argv[1] << "\"\n\n";
            }
        } catch (const Exception &ex){
            log(LogLevel::Error) << "Error running spacepictl " << argv[1] << ": " << ex.what() << "\n" << ex.getPointer();
        }

    } else {
        log(LogLevel::Error) << "Not enough arguments.\n\n";
    }

    log(LogLevel::Info) << "USAGE: " << argv[0] << " <action> [[options] ... ]\n"
            "Available actions:\n"
            "    exec           Execute a SpacePi service\n"
            "    list           List SpacePi services\n"
            "    config-set     Set the next config file\n"
            "\n"
            "    The following are passed directly to systemctl:\n"
            "    start, stop, status, enable, disable, restart, reenable\n";
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
            log(LogLevel::Error) << "Module \"" << argv[2] << "\" not found\n";
        }
    } else {
        log(LogLevel::Error) << "Incorrect number of arguments (" << argv.size() << "). Expects 3.\n\n";
    }

    log(LogLevel::Info) << "USAGE: " << argv[0] << " " << argv[1] << " UNIT_NAME\n";
    return 1;
}


int SpacePiCTL::spacepictl_systemctl(vector<string> argv){
    if(argv.size() == 3){
        spacepi::package::Module module;
        if(get_module(argv[2], module)){
            argv[0] = bp::search_path("systemctl").native();
            argv[2] = System::moduleServiceName(module);

            bp::system(argv);

            return 1;
        } else {
            log(LogLevel::Error) << "Module \"" << argv[2] << "\" not found\n";
        }
    } else {
        log(LogLevel::Error) << "Incorrect number of arguments (" << argv.size() << "). Expects 3.\n\n";
    }

    log(LogLevel::Info) << "USAGE: " << argv[0] << " " << argv[1] << " UNIT_NAME\n";
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
        log(LogLevel::Error) << "Incorrect number of arguments (" << argv.size() << "). Expects 2.\n\n";
    }

    log(LogLevel::Info) << "USAGE: " << argv[0] << " " << argv[1] << "\n";
    return 1;
}

// Do a prefix match on the filename for a path and return matches
vector<fs::path> SpacePiCTL::resolve_path(const fs::path& p){
    fs::path directory = p.parent_path();
    string name = p.filename().native();
    vector<fs::path> matching_paths = {};

    if(fs::exists(directory)){
        for(const fs::path &p2 : fs::directory_iterator(directory)){
            if(!fs::is_directory(p2)){
                string name2 = p2.filename().native();
                const auto &match = std::mismatch(name.begin(), name.end(), name2.begin(), name2.end());
                if(match.first == name.end()){
                    // Given name is prefix to other path
                    if(match.second == name2.end()){
                        // Given name exactly matches given path
                        return {p2};
                    }

                    matching_paths.push_back(p2);
                }
            }
        }
    }

    return matching_paths;
}

vector<string> SpacePiCTL::resolve_config(const string name){
    vector<string> matches = {};
    #ifdef SPACEPI_PACKAGES_FILES
        for(const fs::path &item : {SPACEPI_PACKAGES_FILES}){
            if(item.has_stem() && name == item.stem().native()){
                matches.push_back(item.native());
            } else {
                fs::path name_path(name);
                string reduced_name;
                if(name_path.has_stem()){
                    reduced_name = name_path.parent_path().append(name_path.stem().native()).native();
                } else {
                    reduced_name = name;
                }
                string item_str = item.parent_path().append(item.stem().native()).native();
                const auto &match = std::mismatch(reduced_name.rbegin(), reduced_name.rend(), item_str.rbegin(), item_str.rend());
                if(match.first == reduced_name.rend()){
                    if(match.second == item_str.rend()){
                        return {item.native()};
                    }

                    matches.push_back(item.native());
                }
            }
        }
    #endif
    return matches;
} 

int SpacePiCTL::spacepictl_config_set(std::vector<std::string> argv){
    if(argv.size() == 3){
        vector<fs::path> paths = resolve_path(fs::path(argv[2]));
        if(paths.size() > 1){
            log(LogLevel::Error) << "Ambiguous path. Matches:\n";
            for(const auto &match : paths){
                log(LogLevel::Error) << match.native() << "\n";
            }
            return 1;
        }

        fs::path config_path;

        if(paths.size() == 1){
            config_path = paths[0];
        } else {
            vector<string> configs = resolve_config(argv[2]);
            if(configs.size() > 1){
                log(LogLevel::Error) << "Ambiguous config name. Matches:\n";
                for(const auto &match : configs){
                    log(LogLevel::Error) << match << "\n";
                }
                return 1;
            } else if(configs.size() == 0){
                log(LogLevel::Error) << "Argument does not match any configs\n";
                return 1;
            } else {
                //config_path =  fs::path(SPACEPI_CONFIGS).append(configs[0]);
                config_path =  configs[0];
            }
        }

        string config = fs::canonical(config_path).native();

        log(LogLevel::Info) << "Linking " << NEW_CONF_PATH << " -> " << config << "\n";
        log(LogLevel::Info) << "New configuration will activate on next boot\n";

        FSTransaction fs;
        if(!fs::exists(SPACEPI_ETC)) fs.mkdir(SPACEPI_ETC);
        fs.link(NEW_CONF_PATH, config);
        fs.apply();

        return 0;

    } else {
        log(LogLevel::Error) << "Incorrect number of arguments (" << argv.size() << "). Expects 3.\n\n";
    }

    log(LogLevel::Info) << "USAGE: " << argv[0] << " " << argv[1] << " [ CONFIG_PATH | CONFIG_NAME ]" << "\n";
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
        mount("", "/", NULL, MS_REMOUNT, NULL);
        fs_transaction.apply();
        log(LogLevel::Info) << "Done commiting changes\n";
    } catch (const Exception  &ex) {
        log(LogLevel::Error) << "Error applying changes: " << ex.what() << "\n" << ex.getPointer();
        return 1;
    }

    return 0;
}

