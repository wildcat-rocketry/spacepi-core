#ifndef SPACEPI_MESSAGING_NETWORK_NETWORKTHREAD_HPP
#define SPACEPI_MESSAGING_NETWORK_NETWORKTHREAD_HPP

#include <condition_variable>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <spacepi/log/AutoLog.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            class NetworkThread : private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
                public:
                    static NetworkThread instance;

                    NetworkThread(const NetworkThread &) = delete;

                    NetworkThread &operator =(const NetworkThread &) = delete;

                    boost::asio::io_context &getContext();
                    void start();
                    void join();
                    void stop();

                private:
                    enum State {
                        Starting,
                        Running,
                        Stopping
                    };

                    NetworkThread();
                    ~NetworkThread();

                    void run();

                    boost::asio::io_context ctx;
                    std::mutex mtx;
                    std::condition_variable cond;
                    enum State state;
                    std::thread thread;
            };
        }
    }
}

#endif
