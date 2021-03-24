#include <iostream>
#include <regex>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <spacepi/spacepictl/util/System.hpp>
#include <spacepi/spacepictl/util/FSTransaction.hpp>
#include <spacepi/spacepictl/util/FSOStream.hpp>
#include <spacepi/spacepictl/Config.hpp>
#include <string>
#include <fstream>

using namespace spacepi::spacepictl::util;
using namespace std;
namespace fs = boost::filesystem;


System::System(FSTransaction &fs, const spacepi::package::Options &options, const unordered_map<string, spacepi::package::Module> &modules) : fs(fs), modules(modules) {
    // Read from XML and find needed parameters
    // Get ip and hostname tags
    this->fetch_ip();
    this->fetch_hostname();

    update_hostname = false;
    update_ip = false;
    
    const string new_ip = options.getOption("ip");
    const string new_hostname = options.getOption("hostname");

    if(new_ip != ""){
        if(old_ip != new_ip){
            update_ip = true;
            ip = new_ip;
        }
    }

    if(new_hostname != ""){
        if(old_hostname != new_hostname){
            update_hostname = true;
            hostname = new_hostname;
        }
    }
}

void System::write_updates(){
    if(update_ip){
        this->write_ip();
    }
    if(update_hostname){
        this->write_hostname();
    }

    this->write_services();
}

void System::fetch_ip(){
    ifstream file;
    file.open("/etc/ip");
    if(file.is_open()){
        getline(file, old_ip);
    }
    file.close();
}

void System::fetch_hostname(){
    ifstream file;
    file.open("/etc/hostname");
    if(file.is_open()){
        getline(file, old_hostname);
    }
    file.close();
}

void System::write_ip(){
    FSOStream file(fs, "/etc/ip");
    file << ip << "\n";
}

void System::write_hostname(){
    FSOStream hostname_file(fs, "/etc/hostname");
    hostname_file << hostname << "\n";

    ifstream ifs("/etc/hosts");
    FSOStream hosts_file(fs, "/etc/hosts");

    string line;
    bool found_name = false;
    while(getline(ifs, line)){
        for( size_t p = line.find(old_hostname); p != string::npos; p = line.find( old_hostname, p + hostname.length())){
            line.replace( p, old_hostname.length(), hostname);
            found_name = true;
        }

        hosts_file << line << endl;
    }

    if(!found_name){
        hosts_file << "127.0.0.1	" << hostname << endl;
    }

    ifs.close();
}

void System::delete_service_from_dir(string dir){
    for(auto i = fs::directory_iterator(dir); i != fs::directory_iterator(); i++){
        if(!fs::is_directory(i->path())){
            if(isServiceName(i->path().filename().native())){
                fs.remove(i->path().native());
            }
        }
    }
}

void System::write_services(){
    delete_service_from_dir("/etc/systemd/system/multi-user.target.wants");
    delete_service_from_dir("/lib/systemd/system");

    for(const auto& module_pair : modules){
        spacepi::package::Module module = module_pair.second;
        const string service_file_name = "/lib/systemd/system/" + moduleServiceName(module) + ".service";
        FSOStream service(fs, service_file_name);
        service << "[Unit]\n"
                   "Description=SpacePi Service " << module.getName() << "\n"
                   "\n"
                   "[Service]\n"
                   "Type=exec\n"
                   "ExecStart=" CMAKE_INSTALL_PREFIX "/bin/spacepictl exec " << module.getName() << "\n"
                   "Restart=on-failure\n"
                   "RestartSec=2\n"
                   "User=spacepi\n"
                   "Group=spacepi\n"
                   "UMask=0002\n"
                   "ProtectSystem=strict\n"
                   "ProtectHome=yes\n"
                   "ReadWritePaths=/run /var/local\n";

        if(module.hasAutomaticStart()){
            service << "\n"
                       "[Install]\n"
                       "WantedBy=multi-user.target\n";

            fs.link("/etc/systemd/system/multi-user.target.wants/" + moduleServiceName(module) + ".service", service_file_name);
        }
    }
}

string System::moduleBin(spacepi::package::Module &module){
    string module_type = module.getType();
    replace(module_type.begin(), module_type.end(), '/', '_');
    return CMAKE_INSTALL_PREFIX "/bin/spacepi-mod-" + module_type;
}

string System::moduleServiceName(spacepi::package::Module &module){
    return "spacepi_module_" + module.getName();
}

bool System::isServiceName(const std::string &name){
    // Does string start with this
    return name.rfind("spacepi_module_", 0) == 0;
}
