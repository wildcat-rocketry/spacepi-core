#include <cerrno>
#include <cstring>
#include <exception>
#include <ostream>
#include <string>
#include <vector>
#include <sys/mount.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/FSTransaction.hpp>
#include <spacepi/spacepictl/util/System.hpp>
#include <spacepi/spacepictl/util/UserManager.hpp>
#include <spacepi/spacepictl/Config.hpp>
#include <spacepi/spacepictl/Help.hpp>
#include <spacepi/spacepictl/Init.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::util;
namespace fs = boost::filesystem;

Init Init::instance;

Init::Init() noexcept : Verb("", "Runs the init process") {
}

bool Init::run(const vector<string> &args) {
    bool forceInit = false;
    bool help = false;
    for (vector<string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        if (*it == "--force-init") {
            forceInit = true;
        } else if (*it == "--help") {
            help = true;
        } else {
            printErrorHelp("Unknown option '" + *it + "'.");
            return false;
        }
    }
    if (help) {
        return Help::instance.run({});
    }

    if (getpid() != 1 && !forceInit) {
        printErrorHelp("Not running init process with PID != 1.  Override with --force-init.");
        return false;
    }

    if(!fs::exists(SPACEPI_RUNNING_CONF)){
        //bp::system("mount -a"); // Does not work
        mount("", "/", nullptr, MS_REMOUNT, nullptr);
        mount("/dev/mmcblk0p3", "/var", "ext4", MS_SYNCHRONOUS | MS_NOATIME, NULL);
    }

    try{
        // Check for existence of update flag
        fs::path new_config_path{SPACEPI_NEW_PATH_CONF};
        fs::path canon = fs::canonical(new_config_path);
        if(fs::exists(new_config_path) && fs::is_regular_file(canon)){
            //bp::system("mount -a"); // Does not work
            mount("", "/", nullptr, MS_REMOUNT, nullptr);
            mount("/dev/mmcblk0p3", "/var", "ext4", MS_SYNCHRONOUS | MS_NOATIME, NULL);
            if(!runReconfiguration()){
                FSTransaction fs;

                if(fs::exists(SPACEPI_RUNNING_CONF)) fs.remove(SPACEPI_RUNNING_CONF);
                fs.copy(SPACEPI_NEW_PATH_CONF, SPACEPI_RUNNING_CONF);

                // Delete update flag
                fs.remove(SPACEPI_NEW_PATH_CONF);

                fs.apply();
            }
        }
    } catch (const exception &ex){
        log(LogLevel::Error) << "Error updating system: " << ex.what() << "\n" << Exception::getPointer();
    }

    log(LogLevel::Info) << "Starting /sbin/init";
    // Boot into systemd

/*
    try{
        LogManager::instance.flush();
    } catch (const Exception &ex) {
        log(LogLevel::Error) << "Failed to flush log: " << ex.what() << "\n" << ex.getPointer();
    }
*/

    execl("/sbin/init", "/sbin/init", nullptr); 
    log(LogLevel::Error) << "Error executing /sbin/init: " << strerror(errno) << "!";
    return false;
}

ostream &Init::printHelp(ostream &os) const noexcept {
    return os << "[--force-init] [--help]\n"
                 "Options:\n"
                 "    --force-init  Force this to run in init mode, even if it's PID is not 1\n"
                 "    --help        Prints out a help message\n";
}

bool Init::runReconfiguration() {
    FSTransaction fs_transaction;
    
    spacepi::package::PackageConfig config(SPACEPI_NEW_PATH_CONF);

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
    } catch (const exception &ex) {
        log(LogLevel::Error) << "Error applying changes: " << ex.what() << "\n" << Exception::getPointer();
        return true;
    }

    return false;
}
