#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_CONFIGSET_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_CONFIGSET_HPP

#include <ostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <spacepi/spacepictl/Verb.hpp>

namespace spacepi {
    namespace spacepictl {
        class ConfigSet : public Verb {
            public:
                static ConfigSet instance;

                ConfigSet() noexcept;

            private:
                bool run(const std::vector<std::string> &args);
                std::ostream &printHelp(std::ostream &os) const noexcept;
                std::vector<boost::filesystem::path> resolve_path(const boost::filesystem::path& p);
                std::vector<std::string> resolve_config(const std::string name);
        };
    }
}

#endif
