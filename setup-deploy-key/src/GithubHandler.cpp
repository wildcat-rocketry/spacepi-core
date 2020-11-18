#include <spacepi/setup-deploy-key/GithubHandler.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <spacepi/setup-deploy-key/ServerConn.hpp>

using namespace spacepi::target::deployKey;

GithubHandler::GithubHandler(){

}

void GithubHandler::getFileContents(std::string str){
    filecontents = str;
}

void GithubHandler::sendCurlAPICommand(){
    //erverConnPtr->response.set("Location"," https://api.github.comhttps://api.github.com" + serverConnPtr->httprequest["Authorization: token "].to_string()+ filecontents);
}

void GithubHandler::getUserContent(){
    //std::string content = "curl -H \"Accept: application/vnd.github.v3+json\" https://api.github.com/users";
    //system(content.c_str());
}
