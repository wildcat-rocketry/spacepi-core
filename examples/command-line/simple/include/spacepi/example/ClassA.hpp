#ifndef SPACEPI_EXAMPLE_CLASSA_HPP
#define SPACEPI_EXAMPLE_CLASSA_HPP

#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace example {
        class ClassA : private spacepi::log::AutoLog<decltype("example:A"_autolog)>, private spacepi::util::CommandConfigurable {
            public:
                ClassA(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                bool boolVar;
                int intVar;
                std::string stringVar;
        };
    }
}

#endif
