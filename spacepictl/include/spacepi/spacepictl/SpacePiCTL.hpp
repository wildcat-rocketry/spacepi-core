#ifndef SPACEPI_SPACEPICTL_SPACEPICTL_HPP
#define SPACEPI_SPACEPICTL_SPACEPICTL_HPP

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
                int spacepictl_exec(std::vector<std::string> argv);
                bool get_module(std::string module_name, boost::property_tree::ptree& module_config);

                std::vector<std::string> arg_vector;
        };
    }
}

#endif