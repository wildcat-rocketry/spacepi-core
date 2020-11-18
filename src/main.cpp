#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/setup-deploy-key/Server.hpp>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <spacepi/setup-deploy-key/GithubHandler.hpp>
#include <spacepi/setup-deploy-key/Client.hpp>

using namespace spacepi::messaging::network;
using namespace spacepi::target::deployKey;
using namespace spacepi::log;
using namespace boost::filesystem;

int main(int argc, const char **argv) {
    Server server(8000);
    NetworkThread::instance.start();

    server.waitForCallbackCode();
    std::string callbackcode = server.getCallbackCode();
    Logger log("verification");
    log(LogLevel::Info) << "Callback sent to main";
    log(LogLevel::Info) << callbackcode;

    std::string filepath = std::string(getenv("HOME")) + "/.ssh";
    GithubHandler githubhandler;
    for (const auto & entry : directory_iterator(filepath)){
        std::string p = entry.path().extension().generic_string();
        if (p == ".pub") {
            std::string filename = entry.path().generic_string();
            std::ifstream inputfile;
            inputfile.open(filename);
            std::stringstream stringstream;
            stringstream << inputfile.rdbuf();
            std::string filestring = stringstream.str();
            githubhandler.getFileContents(filestring);
        }
    }
    Client client;
    std::string ret = client.urlRequest("https://api.github.com/user/keys","test","test","test","test");
    log(LogLevel::Info) << ret;

    NetworkThread::instance.join();
    return 0;
}

