#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_EXEC_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_EXEC_HPP

#include <ostream>
#include <string>
#include <vector>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class Exec : public Verb {
            public:
                static Exec instance;

                Exec() noexcept;

            private:
                bool run(const std::vector<std::string> &args);
                std::ostream &printHelp(std::ostream &os) const noexcept;
                char *doAlloc(std::vector<std::vector<char>> &alloc, const std::string &str);
        };
    }
}

#endif
