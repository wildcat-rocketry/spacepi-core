#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_LIST_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_LIST_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class List : public Verb {
            public:
                static List instance;

                List() noexcept;

            private:
                bool run(const std::vector<std::string> &args);
                std::ostream &printHelp(std::ostream &os) const noexcept;
        };
    }
}

#endif
