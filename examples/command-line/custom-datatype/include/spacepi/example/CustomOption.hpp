#ifndef SPACEPI_EXAMPLE_CUSTOMOPTION_HPP
#define SPACEPI_EXAMPLE_CUSTOMOPTION_HPP

#include <spacepi/example/CustomType.hpp>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace example {
        class CustomOption : private spacepi::log::AutoLog<decltype("example"_autolog)>, private spacepi::util::CommandConfigurable {
            public:
                CustomOption(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                CustomType customVar;
        };
    }
}

#endif
