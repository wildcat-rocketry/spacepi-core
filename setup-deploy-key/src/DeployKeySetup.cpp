#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <spacepi/setup-deploy-key/GithubHandler.hpp>
#include <spacepi/setup-deploy-key/Client.hpp>
#include <spacepi/setup-deploy-key/DeployKeySetup.hpp>
#include <SpacePi.hpp>

using namespace spacepi::messaging::network;
using namespace spacepi::target::deployKey;
using namespace spacepi::log;
using namespace boost::filesystem;
using namespace spacepi::util;

DeployKeySetup::DeployKeySetup(Command &cmd) noexcept : CommandConfigurable("Deploy Key Setup Options", cmd) {
    //fromCommand();
}

void DeployKeySetup::runCommand(){
    Server server(8000);
    NetworkThread::instance.start();

    server.waitForCallbackCode();
    std::string callbackcode = server.getCallbackCode();
    Logger log("setup-deploy-key-verification");

    std::string filepath = std::string(getenv("HOME")) + "/.ssh";
    GithubHandler githubhandler(callbackcode);
    for (const auto & entry : directory_iterator(filepath)){
        std::string p = entry.path().extension().generic_string();
        if (p == ".pub") {
            std::string filename = entry.path().generic_string();
            std::ifstream inputfile;
            inputfile.open(filename);
            std::stringstream stringstream;
            stringstream << inputfile.rdbuf();
            std::string filestring = stringstream.str();
            if(! githubhandler.isKey(filestring)){
                githubhandler.addKey(filestring);
            }
        }
    }
    log(LogLevel::Info) << "Done adding keys.";
    
}