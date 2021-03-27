#ifndef SPACEPI_CORE_JOURNALER_FULLCONNECTION_HPP
#define SPACEPI_CORE_JOURNALER_FULLCONNECTION_HPP

#include <functional>
#include <string>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/RawConnection.hpp>
#include <spacepi/util/Command.hpp>

namespace spacepi {
    namespace journaler {
        class FullConnection : public spacepi::messaging::RawConnection {
            public:
                explicit FullConnection(spacepi::util::Command &cmd) noexcept;

                void subscribeAll(const std::function<void(const spacepi::messaging::network::SubscriptionID &, const std::string &)> &func);

            protected:
                void handleMessage(const spacepi::messaging::network::SubscriptionID &id, const std::string &msg);
                void handleConnect();

            private:
                void resubscribe();

                std::function<void(const spacepi::messaging::network::SubscriptionID &, const std::string &)> subscribeFunc;
        };
    }
}

#endif
