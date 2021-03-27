#ifndef SPACEPI_CORE_JOURNALER_PLAYBACK_HPP
#define SPACEPI_CORE_JOURNALER_PLAYBACK_HPP

#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/journaler/FullConnection.hpp>
#include <spacepi/journaler/Journal.hpp>

namespace spacepi {
    namespace journaler {
        class Playback : private spacepi::util::CommandConfigurable {
            public:
                Playback(spacepi::util::Command &cmd, Journal &journal, FullConnection &conn) noexcept;

            private:
                void runCommand();

                Journal &journal;
                FullConnection &conn;
                bool enable;
        };
    }
}

#endif
