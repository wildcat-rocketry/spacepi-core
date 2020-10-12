#ifndef SPACEPI_CORE_UTIL_COMMANDCONFIGURABLE_HPP
#define SPACEPI_CORE_UTIL_COMMANDCONFIGURABLE_HPP

#include <string>
#include <vector>
#include <boost/program_options.hpp>

namespace spacepi {
    namespace util {
        class CommandConfigurable {
            public:
                CommandConfigurable(const std::string &caption, std::vector<std::string> &args);
                virtual ~CommandConfigurable();

                static std::vector<std::string> parse(int argc, const char **argv);
            
            protected:
                void construct();
                virtual void options(boost::program_options::options_description &desc) const = 0;
                virtual void configure(const boost::program_options::parsed_options &opts) = 0;

            private:
                const std::string &caption;
                std::vector<std::string> &args;
                bool constructed;
        };
    }
}

#endif
