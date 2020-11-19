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

            class SocketWrapperCallback {
                friend class detail::GenericSocketReader;
                friend class detail::GenericSocketWriter;

                protected:
                    virtual void handlePacket(const std::string &pkt) = 0;
                    virtual void handleError(const spacepi::util::Exception::pointer &err) = 0;
            };
        }
    }
}

#endif
