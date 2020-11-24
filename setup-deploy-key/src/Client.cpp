#include <spacepi/setup-deploy-key/Client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

using namespace spacepi::target::deployKey;
using namespace boost::beast;
using namespace boost::beast::http;
using namespace boost::asio;
using namespace spacepi::messaging::network;
using namespace boost::asio::ip;
using namespace boost::property_tree::json_parser;

Client::Client() : ctx(ssl::context::sslv23_client) {
    std::string file = "/etc/ssl/certs/ca-certificates.crt";
    std::ifstream stream;
    stream.open(file);
    std::stringstream stringstream;
    stringstream << stream.rdbuf();
    std::string fileofcrap = stringstream.str();
    ctx.add_certificate_authority(buffer(fileofcrap.data(),fileofcrap.size())); 
}

std::string Client::urlRequest(std::string url, http::verb method, std::string body, std::string contenttype, std::string accept,std::string authorization){
    tcp::resolver resolver(NetworkThread::instance.getContext());
    ssl::stream<tcp::socket> socket{NetworkThread::instance.getContext(), ctx};

    std::string host;
    std::string path;
    int slash = url.find_first_of('/');
    slash = url.find_first_of('/',slash+1);
    host = url.substr(slash+1);
    slash = host.find_first_of('/');
    path = host.substr(slash);
    host = host.substr(0,slash); 
    std::string protocol = url.substr(0,5);
    tcp::resolver::results_type results;
    if(protocol == "https:"){
        results = resolver.resolve(host, "443");
    }
    else
    {
        results = resolver.resolve(host,"80");
    }
    

    http::request<http::string_body> req{method,path.c_str(), 11};

    if(! SSL_set_tlsext_host_name(socket.native_handle(), host.data()))
    {
        boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        throw boost::system::system_error{ec};
    }

    boost::asio::connect(socket.next_layer(), results.begin(), results.end());
    socket.handshake(ssl::stream_base::client);

    req.set(http::field::host, host.c_str());
    req.set(http::field::content_type,contenttype.c_str());
    req.set(http::field::accept,accept.c_str());
    if(!authorization.empty()){
        req.set(http::field::authorization,authorization.c_str());
    }
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.body() = body;

    http::write(socket, req);
    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(socket, buffer, res);
    boost::system::error_code ec;
    socket.shutdown(ec);

    return res.body();
}

boost::property_tree::ptree Client::urlRequest(std::string url, boost::beast::http::verb method, boost::property_tree::ptree body, std::string contenttype, std::string accept,std::string authorization){
    std::stringstream stringstream;
    write_json(stringstream,body, false);
    std::string bodystr = stringstream.str();
    while (bodystr.back() == '\n') {
        bodystr = bodystr.substr(0, bodystr.length() - 1);
    }
    std::string reqbody = urlRequest(url,method,bodystr,contenttype,accept,authorization);
    stringstream.str(reqbody);
    boost::property_tree::ptree tree;
    read_json(stringstream,tree);
    return tree;
}