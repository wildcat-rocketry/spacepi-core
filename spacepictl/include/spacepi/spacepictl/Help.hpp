#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_HELP_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_HELP_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class Help : public Verb {
            public:
                static Help instance;

                Help() noexcept;

                bool run(const std::vector<std::string> &args);

            private:
                std::ostream &printHelp(std::ostream &os) const noexcept;
        };
    }
}

#endif
