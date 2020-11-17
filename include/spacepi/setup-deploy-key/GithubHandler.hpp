#ifndef SPACEPI_TARGET_GITHUBHANDLER_HPP
#define SPACEPI_TARGET_GITHUBHANDLER_HPP

#include <fstream>
#include <iostream>
#include <sstream>

namespace spacepi{
    namespace target {
        namespace deployKey {
            class GithubHandler{
            public:
            GithubHandler();
            GithubHandler(const GithubHandler &) = delete; 
            GithubHandler & operator = (const GithubHandler &) = delete; 
            void getFileContents(std::string str);
            void sendCurlAPICommand();
            void getUserContent();

            private:
            std::string filecontents;
            };
        }
    }
}
#endif