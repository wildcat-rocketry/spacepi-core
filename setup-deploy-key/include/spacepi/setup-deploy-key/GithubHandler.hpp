#ifndef SPACEPI_TARGET_GITHUBHANDLER_HPP
#define SPACEPI_TARGET_GITHUBHANDLER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <spacepi/setup-deploy-key/Client.hpp>
#include <unordered_set>

namespace spacepi{
    namespace target {
        namespace deployKey {
            class GithubHandler: private spacepi::log::AutoLog<decltype("setup-deploy-key"_autolog)>{
            public:
            GithubHandler(std::string callbackcode);
            GithubHandler(const GithubHandler &) = delete; 
            GithubHandler & operator = (const GithubHandler &) = delete;
            bool isKey(std::string key);
            void addKey(std::string key);

            private:
            Client client;
            std::unordered_set<std::string> keys;
            std::string accesstoken;
            bool handleErrors(boost::property_tree::ptree tree);
            };
        }
    }
}
#endif
