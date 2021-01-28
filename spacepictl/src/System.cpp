#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <regex>
#include <spacepi/spacepictl/System.hpp>
#include <string>
#include <fstream>


using boost::property_tree::ptree;
using boost::optional;
namespace fs = boost::filesystem;
using namespace spacepi::spacepictl;
using namespace std;

System::System(ptree & config){
    // Read from XML and find needed parameters
    // Get ip and hostname tags
    this->fetch_ip();
    this->fetch_hostname();

    update_hostname = false;
    update_ip = false;
    
    optional<string> new_ip = config.get_optional<string>("ip");
    optional<string> new_hostname = config.get_optional<string>("hostname");

    if(new_ip){
        if(ip.compare(*new_ip) != 0){
            update_ip = true;
            ip = *new_ip;
        }
    }

    if(new_hostname){
        if(hostname.compare(*new_hostname) != 0){
            update_hostname = true;
            hostname = *new_hostname;
        }
    }
}

bool System::needs_update(){
    return update_hostname || update_ip;
}

void System::write_updates(){
    if(update_ip){
        this->write_ip();
    }
    if(update_hostname){
        this->write_hostname();
    }
}

void System::fetch_ip(){
    ifstream file;
    file.open("/etc/ip");
    if(file.is_open()){
        getline(file, ip);
    }
}

void System::fetch_hostname(){
    ifstream file;
    file.open("/etc/hostname");
    if(file.is_open()){
        getline(file, hostname);
    }
}

void System::write_ip(){
    ofstream file;
    file.open("/etc/ip");
    file << ip << "\n";
    file.close();
}

void System::write_hostname(){
    ofstream file;
    file.open("/etc/hostname");
    file << hostname << "\n";
    file.close();

    ifstream ifs("/etc/hosts");
    string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
    ifs.close();

    file.open("/etc/hosts");
    file << regex_replace(content, regex(old_hostname), hostname);
    file.close();
}
