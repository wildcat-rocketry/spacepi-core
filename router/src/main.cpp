#include <boost/asio.hpp>
#include <spacepi/router/RouterServer.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace spacepi::router;

int main(int argc, const char **argv) {
    io_context ctx;
    RouterServer<tcp> server(ctx, tcp::endpoint(tcp::v4(), 8000));
    ctx.run();
    return 0;
}
