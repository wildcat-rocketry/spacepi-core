#include <spacepi/setup-deploy-key/GithubHandler.hpp>

using namespace spacepi::target::deployKey;

GithubHandler::GithubHandler(){

}

void GithubHandler::getFileContents(std::string str){
    filecontents = str;
}
