#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SSHGEN_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SSHGEN_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class SSHGen : public Verb {
            public:
                static SSHGen instance;

                SSHGen() noexcept;

                bool run(const std::vector<std::string> &args);

            private:
                void generate(const std::string &privateKey, const std::string &type, const std::string &comment) noexcept;

                std::ostream &printHelp(std::ostream &os) const noexcept;

                bool success;
        };
    }
}

#endif
