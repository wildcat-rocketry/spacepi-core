#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <regex>
#include <spacepi/spacepictl/System.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>
#include <spacepi/spacepictl/FSOStream.hpp>
#include <string>
#include <fstream>


using boost::property_tree::ptree;
using boost::optional;
namespace fs = boost::filesystem;
using namespace spacepi::spacepictl;
using namespace std;

System::System(FSTransaction &fs, ptree & config) : fs(fs) {
    // Read from XML and find needed parameters
    // Get ip and hostname tags
    this->fetch_ip();
    this->fetch_hostname();

    update_hostname = false;
    update_ip = false;
    
    optional<string> new_ip = config.get_optional<string>("ip");
    optional<string> new_hostname = config.get_optional<string>("hostname");

    if(new_ip){
        if(old_ip != *new_ip){
            update_ip = true;
            ip = *new_ip;
        }
    }

    if(new_hostname){
        if(old_hostname != *new_hostname){
            update_hostname = true;
            hostname = *new_hostname;
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
    string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
    ifs.close();

    FSOStream hosts_file(fs, "/etc/hosts");
    hosts_file << regex_replace(content, regex(old_hostname), hostname);
}
