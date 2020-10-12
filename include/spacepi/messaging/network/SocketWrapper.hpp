#ifndef SPACEPI_CORE_MESSAGING_NETWORK_SOCKETWRAPPER_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_SOCKETWRAPPER_HPP

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            class GenericSocketReader;
            class GenericSocketWriter;

            class SocketWrapperCallback {
                friend class GenericSocketReader;
                friend class GenericSocketWriter;

                protected:
                    virtual void handlePacket(const std::string &pkt) = 0;
                    virtual void handleError(spacepi::util::Exception::pointer err) = 0;
            };

            class DefaultSocketWrapperCallback : public SocketWrapperCallback, private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
                protected:
                    virtual void handleError(spacepi::util::Exception::pointer err);
            };

            class GenericSocketWrapper;

            class GenericSocketReader {
                friend class GenericSocketWrapper;

                public:
                    std::pair<boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type>, bool> operator ()(boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> begin, boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> end);
                    void operator ()(const boost::system::error_code &err, size_t length);

                private:
                    GenericSocketReader(GenericSocketWrapper *wrapper);

                    GenericSocketWrapper *wrapper;
            };

            class GenericSocketWriter {
                friend class GenericSocketWrapper;

                public:
                    void operator ()(const boost::system::error_code &err, size_t length);

                private:
                    GenericSocketWriter(GenericSocketWrapper *wrapper);

                    GenericSocketWrapper *wrapper;
            };

            class GenericSocketWrapper : private spacepi::log::AutoLog<decltype("core:messaging"_autolog)> {
                friend class GenericSocketReader;
                friend class GenericSocketWriter;

                public:
                    GenericSocketWrapper(SocketWrapperCallback *callback);
                    GenericSocketWrapper(const GenericSocketWrapper &) = delete;

                    GenericSocketWrapper &operator =(const GenericSocketWrapper &) = delete;

                    virtual void startRead() = 0;
                    void sendPacket(const std::string &pkt);

                protected:
                    virtual void startWrite() = 0;

                    GenericSocketReader reader;
                    GenericSocketWriter writer;
                    boost::asio::streambuf readBuffer;
                    boost::asio::streambuf writeBuffer;

                private:
                    SocketWrapperCallback *callback;
                    bool inited;
                    std::mutex writeMutex;
                    int writeCommitted;
                    int writeUncommitted;
                    bool isWriting;
            };

            template <typename Proto>
            class SocketWrapper : public GenericSocketWrapper {
                public:
                    SocketWrapper(SocketWrapperCallback *callback);

                    boost::asio::basic_stream_socket<Proto> &getSocket();

                protected:
                    void startRead();
                    void startWrite();

                private:
                    std::shared_ptr<boost::asio::basic_stream_socket<Proto>> socket;
            };
        }
    }
}

namespace boost {
    namespace asio {
        template <>
        struct is_match_condition<spacepi::messaging::network::GenericSocketReader> : boost::true_type {
        };
    }
}

#endif
