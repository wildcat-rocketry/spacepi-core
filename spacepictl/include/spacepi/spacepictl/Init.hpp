#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_INIT_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_INIT_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class Init : public Verb {
            public:
                static Init instance;

                Init() noexcept;

            private:
                bool run(const std::vector<std::string> &args);
                std::ostream &printHelp(std::ostream &os) const noexcept;

                bool runReconfiguration();
        };
    }
}

#endif
