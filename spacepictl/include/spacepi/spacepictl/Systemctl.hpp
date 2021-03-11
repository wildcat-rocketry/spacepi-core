#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SYSTEMCTL_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SYSTEMCTL_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class Systemctl : public Verb {
            public:
                static Systemctl disable;
                static Systemctl enable;
                static Systemctl reenable;
                static Systemctl restart;
                static Systemctl start;
                static Systemctl status;
                static Systemctl stop;

                Systemctl(const std::string &verb) noexcept;

            private:
                bool run(const std::vector<std::string> &args);
                std::ostream &printHelp(std::ostream &os) const noexcept;

                std::vector<char> verb;
        };
    }
}

#endif
