#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SPACEPICTL_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_SPACEPICTL_HPP

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <SpacePi.hpp>
#include <vector>

namespace spacepi {
    namespace spacepictl {
        class SpacePiCTL : spacepi::log::AutoLog<decltype("spacepictl"_autolog)> {
            public:

                int initialize_system();
                int userspace_utility(std::vector<std::string> argv);
                int run_reconfiguration();

                int main(std::vector<std::string> argv);

            private:

                bool points_to_file(boost::filesystem::path& p);
                bool get_module(std::string module_name, spacepi::package::Module &module_config);

                int spacepictl_exec(std::vector<std::string> argv);
                int spacepictl_list(std::vector<std::string> argv);

                std::vector<std::string> arg_vector;
        };
    }
}

#endif