#ifndef SPACEPI_TARGET_CLIENT_HPP
#define SPACEPI_TARGET_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <SpacePi.hpp>

namespace spacepi{
    namespace target {
        namespace deployKey {
            class Client{
                public:
                    Client();
                    Client(const Client &) = delete; 
                    Client & operator = (const Client &) = delete;
                    std::string urlRequest(std::string url, boost::beast::http::verb method, std::string body, std::string contenttype, std::string accept,std::string authorization);
                    boost::property_tree::ptree urlRequest(std::string url, boost::beast::http::verb method, boost::property_tree::ptree body, std::string contenttype, std::string accept,std::string authorization);

                private:
                    boost::asio::ssl::context ctx;
            };
        }
    }
}
#endif
