#ifndef SPACEPI_TARGET_CLIENT_HPP
#define SPACEPI_TARGET_CLIENT_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace spacepi{
    namespace target {
        namespace deployKey {
            class Client{
                public:
                    Client();
                    Client(const Client &) = delete; 
                    Client & operator = (const Client &) = delete;
                    //boost::property_tree::ptree getPtreeResponse(boost::property_tree::ptree request);
                    std::string urlRequest(std::string url, std::string method, std::string body, std::string contenttype, std::string accept);


                private:
                //boost::property_tree::ptree response;
                boost::asio::ssl::context ctx;
            };
        }
    }
}
#endif
