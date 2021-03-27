#ifndef SPACEPI_CORE_JOURNALER_JOURNALER_HPP
#define SPACEPI_CORE_JOURNALER_JOURNALER_HPP

#include <chrono>
#include <string>
#include <queue>
#include <spacepi/concurrent/ConditionVariable.hpp>
#include <spacepi/messaging/network/SubscriptionID.hpp>
#include <spacepi/messaging/JournalEntry.pb.h>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/journaler/FullConnection.hpp>
#include <spacepi/journaler/Journal.hpp>

namespace spacepi {
    namespace journaler {
        class Journaler : private spacepi::util::CommandConfigurable {
            public:
                Journaler(spacepi::util::Command &cmd, Journal &journal, FullConnection &conn) noexcept;

            private:
                void handleMessage(const spacepi::messaging::network::SubscriptionID &id, const std::string &msg);
                void runCommand();

                Journal &journal;
                FullConnection &conn;
                bool enable;
                std::chrono::steady_clock::time_point start;
                std::queue<spacepi::messaging::detail::JournalEntry> queue;
                spacepi::concurrent::ConditionVariable cond;
        };
    }
}

#endif
