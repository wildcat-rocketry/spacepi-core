#ifndef SPACEPI_TARGET_RPI_USERMANAGER_HPP
#define SPACEPI_TARGET_RPI_USERMANAGER_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace target {
        namespace rpi {
            class UserManager {
                public:
                    UserNanager();
                    void AddOrUpdateUser(std::string uname,
                                         boost::optional<std::string> name, boost::optional<std::string> email,
                                         boost::optional<std::string> shell, boost::optional<std::string> keys);

                protected:
                    void options(boost::program_options::options_description &desc) const;
                    void configure(const boost::program_options::parsed_options &opts);

                private:
                    std::list<spacepi::target::rpi::Users> system_users;
                    std::list<spacepi::target::rpi::People> human_users;
            };
        }
    }
}

#endif
