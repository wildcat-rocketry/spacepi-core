#ifndef SPACEPI_SPACEPICTL_SPACEPICTL_HPP
#define SPACEPI_SPACEPICTL_SPACEPICTL_HPP

#include <boost/filesystem.hpp>
#include <SpacePi.hpp>

namespace spacepi {
    namespace spacepictl {
        class SpacePiCTL : spacepi::log::AutoLog<decltype("spacepictl"_autolog)> {
            public:

                int initialize_system();
                int userspace_utility();
                int run_reconfiguration();

                int main(int argc, char ** argv);

            private:

                bool points_to_file(boost::filesystem::path& p);

                int arg_count;
                char ** arg_vector;
        };
    }
}

#endif