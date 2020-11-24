#ifndef SPACEPI_EXAMPLE_CLASSB_HPP
#define SPACEPI_EXAMPLE_CLASSB_HPP

#include <string>
#include <vector>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace example {
        class ClassB : private spacepi::log::AutoLog<decltype("example:B"_autolog)>, private spacepi::util::CommandConfigurable {
            public:
                ClassB(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                bool boolVar;
                std::vector<std::string> stringList;
        };
    }
}

#endif
