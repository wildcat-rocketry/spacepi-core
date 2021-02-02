#ifndef SPACEPI_TARGET_RPI_SYSTEM_HPP
#define SPACEPI_TARGET_RPI_SYSTEM_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/FSTransaction.hpp>

namespace spacepi {
    namespace spacepictl {
        class System : private spacepi::log::AutoLog<decltype("spacepictl:System"_autolog)> {
            public:
                System(spacepi::spacepictl::FSTransaction &fs, boost::property_tree::ptree & config);

                void write_updates();

            protected:

            private:
                bool update_hostname;
                bool update_ip;

                std::string ip;
                std::string hostname;

                std::string old_ip;
                std::string old_hostname;

                spacepi::spacepictl::FSTransaction &fs;

                void fetch_ip();
                void fetch_hostname();

                void write_ip();
                void write_hostname();

        };
    }
}

#endif
