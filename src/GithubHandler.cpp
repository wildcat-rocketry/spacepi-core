#include <spacepi/setup-deploy-key/GithubHandler.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/setup-deploy-key/Client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <boost/property_tree/exceptions.hpp>

using namespace spacepi::target::deployKey;
using namespace boost::property_tree;
using namespace spacepi::log;

GithubHandler::GithubHandler(std::string callbackcode){
    ptree bodytree;
    bodytree.put("client_id","6a1a713dd92598462dec");
    bodytree.put("client_secret","fe8615e958d230646d9bfc35e57dcc446b903fe9");
    bodytree.put("code",callbackcode);
    ptree response;
    try {
        response = client.urlRequest("https://github.com/login/oauth/access_token", boost::beast::http::verb::post, bodytree, "application/json", "application/json","");
        accesstoken = response.get<std::string>("access_token");
        response = client.urlRequest("https://api.github.com/user/keys", boost::beast::http::verb::get, ptree(), "", "application/vnd.github.v3+json","token " + accesstoken);
        for (ptree::const_iterator it = response.begin(); it != response.end(); ++it) {
            keys.insert(it->second.get<std::string>("key"));
        }
    }
    catch(const ptree_error &e) {  
        Logger log("debugger");
        log(LogLevel::Error) << e.what();
    }
}


bool GithubHandler::isKey(std::string key){
    return keys.find(key) != keys.end();
}

void GithubHandler::addKey(std::string key){
    ptree tree;
    tree.put("key",key);
    client.urlRequest("https://api.github.com/user/keys", boost::beast::http::verb::post, tree, "application/vnd.github.v3+json", "application/vnd.github.v3+json","token " + accesstoken);
}
