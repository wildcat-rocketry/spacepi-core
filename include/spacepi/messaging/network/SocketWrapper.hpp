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

            /**
             * \brief Base class for SocketWrapper which allows polymorphic access
             */
            class GenericSocketWrapper : public std::enable_shared_from_this<GenericSocketWrapper> {
                friend class detail::GenericSocketReader;
                friend class detail::GenericSocketWriter;

                public:
                    /**
                     * \brief Initialize the GenericSocketWrapper
                     * 
                     * \param[in] callback The callback to call when events occur
                     */
                    explicit GenericSocketWrapper(const spacepi::util::WeakOrRef<SocketWrapperCallback> &callback);

                    GenericSocketWrapper(GenericSocketWrapper &) = delete;
                    GenericSocketWrapper &operator =(GenericSocketWrapper &) = delete;

                    /**
                     * \brief Start reading data from the socket asynchronously
                     */
                    virtual void startRead() = 0;

                    /**
                     * \brief Sends a length-prefixed packet to the SocketWrapper on the other side of the connection
                     * 
                     * When the message is received, it will get passed to the SocketWrapperCallback::handlePacket
                     * 
                     * \param[in] pkt The serialized binary representation of the packet
                     */
                    void sendPacket(const std::string &pkt);

                protected:
                    /**
                     * \brief Begins an asynchronous write operation on the socket for any currently buffered data
                     */
                    virtual void startWrite() = 0;

                    /**
                     * \brief The buffer of data which has been read from the socket
                     */
                    boost::asio::streambuf readBuffer;

                    /**
                     * \brief The buffer of data which needs to be written to the socket
                     */
                    boost::asio::streambuf writeBuffer;

                    /**
                     * \brief The callback to use for this SocketWrapper's events
                     */
                    spacepi::util::WeakOrRef<SocketWrapperCallback> callback;

                private:
                    bool inited;
                    std::mutex writeMutex;
                    std::queue<std::string> writeUncommitted;
                    int writeCommitted;
                    bool isWriting;
            };

            /**
             * \brief A wrapper around a socket which sends and receives length-prefixed packets
             * 
             * To use multiple protocols polymorphically, see GenericSocketWrapper
             * 
             * This class is defined for the following protocols:
             * \li \c boost::asio::ip::tcp
             * \li \c boost::asio::local::stream_protocol
             */
            template <typename Proto>
            class SocketWrapper : public GenericSocketWrapper {
                public:
                    /**
                     * \brief Initialize the SocketWrapper
                     * 
                     * \param[in] callback The callback to call when events occur
                     */
                    explicit SocketWrapper(const spacepi::util::WeakOrRef<SocketWrapperCallback> &callback);

                    SocketWrapper(SocketWrapper &) = delete;
                    SocketWrapper &operator =(SocketWrapper &) = delete;

                    /**
                     * \brief Get the underlying socket instance
                     * 
                     * \return The raw socket
                     */
                    typename Proto::socket &getSocket();

                protected:
                    /**
                     * \brief Begins an asynchronous read operation on the socket
                     */
                    void startRead();

                    /**
                     * \brief Begins an asynchronous write operation on the socket for any currently buffered data
                     */
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
