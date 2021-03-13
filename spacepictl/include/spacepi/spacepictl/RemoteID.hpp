#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_REMOTEID_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_REMOTEID_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class RemoteID : public Verb {
            public:
                static RemoteID instance;

                RemoteID() noexcept;

            private:
                bool run(const std::vector<std::string> &args);
                std::ostream &printHelp(std::ostream &os) const noexcept;
        };
    }
}

#endif
