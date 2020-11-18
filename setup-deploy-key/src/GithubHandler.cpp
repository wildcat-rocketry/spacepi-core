#include <spacepi/setup-deploy-key/GithubHandler.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <spacepi/setup-deploy-key/ServerConn.hpp>
#include <spacepi/setup-deploy-key/Client.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace spacepi::target::deployKey;
using namespace boost::property_tree;

GithubHandler::GithubHandler(std::string callbackcode){
    ptree tree;
    tree.put("client_id","6a1a713dd92598462dec");
    tree.put("client_secret","fe8615e958d230646d9bfc35e57dcc446b903fe9");
    tree.put("code",callbackcode);
    ptree response;
    response = client.urlRequest("https://github.com/login/oauth/access_token", boost::beast::http::verb::post, tree, "application/json", "application/json","");
    std::string accesstoken;
    accesstoken = response.get<std::string>("access_token");
    response = client.urlRequest("https://api.github.com/user/keys", boost::beast::http::verb::get, ptree(), "", "application/vnd.github.v3+json","token " + accesstoken);
    //keys = response.get<std::string>("key");
}
