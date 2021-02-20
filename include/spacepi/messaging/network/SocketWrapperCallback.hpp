#ifndef SPACEPI_CORE_MESSAGING_NETWORK_SOCKETWRAPPERCALLBACK_HPP
#define SPACEPI_CORE_MESSAGING_NETWORK_SOCKETWRAPPERCALLBACK_HPP

#include <string>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace messaging {
        namespace network {
            namespace detail {
                class GenericSocketReader;
                class GenericSocketWriter;
            }

            /**
             * \brief Callback class for events from the SocketWrapper
             */
            class SocketWrapperCallback {
                friend class detail::GenericSocketReader;
                friend class detail::GenericSocketWriter;

                public:
                    /**
                     * \brief Destroy this SocketWrapperCallback
                     */
                    virtual ~SocketWrapperCallback() = default;

                protected:
                    /**
                     * \brief Called after a message from the other end of the socket is received
                     * 
                     * \param[in] pkt The serialized binary representation of the packet
                     */
                    virtual void handlePacket(const std::string &pkt) = 0;

                    /**
                     * \brief Called any time there is an error during an IO operation with the socket
                     * 
                     * \param[in] err The error which occurred
                     */
                    virtual void handleError(const spacepi::util::Exception::pointer &err) = 0;
            };
        }
    }
}

#endif
