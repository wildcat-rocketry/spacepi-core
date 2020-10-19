#ifndef SPACEPI_TARGET_RPI_OSBUILDER_HPP
#define SPACEPI_TARGET_RPI_OSBUILDER_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace target {
        namespace rpi {
            class OSBuilder : public spacepi::util::CommandConfigurable {
                public:
                    OSBuilder(std::vector<std::string> &args);

                    void run();

                protected:
                    void options(boost::program_options::options_description &desc) const;
                    void configure(const boost::program_options::parsed_options &opts);

                private:
                    std::string configFile;
                    std::string outFile;
                    std::string dataDir;
            };
        }
    }
}

#endif
