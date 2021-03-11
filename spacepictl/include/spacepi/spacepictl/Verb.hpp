#ifndef SPACEPI_TARGETLIB_LINUX_SPACEPICTL_VERB_HPP
#define SPACEPI_TARGETLIB_LINUX_SPACEPICTL_VERB_HPP

#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <SpacePi.hpp>

namespace spacepi {
    namespace spacepictl {
        class Help;

        class Verb {
            friend class Help;

            public:
                virtual ~Verb() = default;

                static int main(int argc, const char **argv) noexcept;
                static std::ostream &printFullHelp(std::ostream &os) noexcept;

            protected:
                Verb(const std::string &name, const std::string &desc) noexcept;

                void printErrorHelp(const std::string &error) const noexcept;

                static spacepi::log::Logger log;

            private:
                static std::unordered_map<std::string, Verb *> &getVerbs() noexcept;

                virtual bool run(const std::vector<std::string> &args) = 0;
                virtual std::ostream &printHelp(std::ostream &os) const noexcept = 0;

                std::string name;
                std::string desc;
                static std::string argv0;
        };
    }
}

#endif
