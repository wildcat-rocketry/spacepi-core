#ifndef SPACEPI_CORE_MEMORY_SOCKETDELETOR_HPP
#define SPACEPI_CORE_MEMORY_SOCKETDELETOR_HPP

#include <boost/asio.hpp>

namespace spacepi {
    namespace memory {
        class SocketDeleter {
            public:
                SocketDeleter();

                template <typename Proto>
                void operator ()(boost::asio::basic_socket<Proto> *ptr) const;

                template <typename Proto>
                void operator ()(boost::asio::basic_stream_socket<Proto> *ptr) const;
        };
    }
}

#endif
