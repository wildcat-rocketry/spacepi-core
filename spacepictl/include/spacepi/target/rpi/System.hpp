#ifndef SPACEPI_TARGET_RPI_SYSTEM_HPP
#define SPACEPI_TARGET_RPI_SYSTEM_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

namespace spacepi {
    namespace target {
        namespace rpi {
            class System {
                public:
                    System(boost::property_tree::ptree & config);

                    bool needs_update();

                    void write_updates();

                protected:

                private:
                    bool update_hostname;
                    bool update_ip;

                    std::string ip;
                    std::string hostname;
                    std::string old_hostname;

                    void fetch_ip();
                    void fetch_hostname();

                    void write_ip();
                    void write_hostname();

            };
        }
    }
}

#endif
