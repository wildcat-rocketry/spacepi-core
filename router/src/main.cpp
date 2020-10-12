#include <boost/asio.hpp>
#include <spacepi/messaging/network/NetworkThread.hpp>
#include <spacepi/router/PubSubRouter.hpp>
#include <spacepi/router/StreamServer.hpp>

using namespace boost::asio::ip;
using namespace spacepi::messaging::network;
using namespace spacepi::router;

int main(int argc, const char **argv) {
    PubSubRouter router;
    StreamServer<tcp> server(&router, tcp::endpoint(tcp::v4(), 8000));
    NetworkThread::instance.join();
    return 0;
}
