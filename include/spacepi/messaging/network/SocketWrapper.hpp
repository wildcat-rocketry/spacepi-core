#ifndef SPACEPI_CORE_MESSAGING_NETWORK_SOCKETWRAPPER_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_SOCKETWRAPPER_HPP

#include <cstddef>
#include <mutex>
#include <queue>
#include <string>
#include <utility>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <spacepi/messaging/network/SocketWrapperCallback.hpp>
#include <spacepi/util/SharedOrRef.hpp>
#include <spacepi/util/WeakOrRef.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            class GenericSocketWrapper;

            template <typename Proto>
            class SocketWrapper;

            namespace detail {

                class GenericSocketReader {
                    template <typename Proto>
                    friend class network::SocketWrapper;

                    public:
                        std::pair<boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type>, bool> operator ()(boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> begin, boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> end);
                        void operator ()(const boost::system::error_code &err, size_t length);

                    private:
                        explicit GenericSocketReader(GenericSocketWrapper &wrapper, spacepi::util::SharedOrRef<SocketWrapperCallback> &&callback) noexcept;

                        spacepi::util::SharedOrRef<GenericSocketWrapper> wrapper;
                        spacepi::util::SharedOrRef<SocketWrapperCallback> callback;
                };

                class GenericSocketWriter {
                    template <typename Proto>
                    friend class network::SocketWrapper;

                    public:
                        void operator ()(const boost::system::error_code &err, size_t length);

                    private:
                        explicit GenericSocketWriter(GenericSocketWrapper &wrapper, spacepi::util::SharedOrRef<SocketWrapperCallback> &&callback) noexcept;

                        spacepi::util::SharedOrRef<GenericSocketWrapper> wrapper;
                        spacepi::util::SharedOrRef<SocketWrapperCallback> callback;
                };
            }

            class GenericSocketWrapper : public std::enable_shared_from_this<GenericSocketWrapper> {
                friend class detail::GenericSocketReader;
                friend class detail::GenericSocketWriter;

                public:
                    explicit GenericSocketWrapper(const spacepi::util::WeakOrRef<SocketWrapperCallback> &callback);

                    GenericSocketWrapper(GenericSocketWrapper &) = delete;
                    GenericSocketWrapper &operator =(GenericSocketWrapper &) = delete;

                    virtual void startRead() = 0;
                    void sendPacket(const std::string &pkt);

                protected:
                    virtual void startWrite() = 0;

                    boost::asio::streambuf readBuffer;
                    boost::asio::streambuf writeBuffer;
                    spacepi::util::WeakOrRef<SocketWrapperCallback> callback;

                private:
                    bool inited;
                    std::mutex writeMutex;
                    std::queue<std::string> writeUncommitted;
                    int writeCommitted;
                    bool isWriting;
            };

            template <typename Proto>
            class SocketWrapper : public GenericSocketWrapper {
                public:
                    explicit SocketWrapper(const spacepi::util::WeakOrRef<SocketWrapperCallback> &callback);

                    SocketWrapper(SocketWrapper &) = delete;
                    SocketWrapper &operator =(SocketWrapper &) = delete;

                    typename Proto::socket &getSocket();

                protected:
                    void startRead();
                    void startWrite();

                private:
                    typename Proto::socket socket;
            };
        }
    }
}

namespace boost {
    namespace asio {
        template <>
        struct is_match_condition<spacepi::messaging::network::detail::GenericSocketReader> : boost::true_type {
        };
    }
}

#endif
