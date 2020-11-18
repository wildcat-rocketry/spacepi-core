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
            class GithubHandler{
            public:
            GithubHandler(std::string callbackcode);
            GithubHandler(const GithubHandler &) = delete; 
            GithubHandler & operator = (const GithubHandler &) = delete; 

            bool hasKey(const std::string &key);
            void addKey(const std::string &key);

            private:
            Client client;
            std::unordered_set<std::string> keys;
            };
        }
    }
}
#endif
