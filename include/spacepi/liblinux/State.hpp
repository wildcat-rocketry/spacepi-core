#ifndef SPACEPI_TARGETLIB_LINUX_STATE_HPP
#define SPACEPI_TARGETLIB_LINUX_STATE_HPP

#include <string>
#include <spacepi/liblinux/StateData.pb.h>

namespace spacepi {
    namespace liblinux {
        class State {
            public:
                State() = default;
                State(const std::string &ser);

                operator std::string() const;

                StateData *operator ->() noexcept;
                const StateData *operator ->() const noexcept;

            private:
                StateData data;
        };
    }
}

#endif
