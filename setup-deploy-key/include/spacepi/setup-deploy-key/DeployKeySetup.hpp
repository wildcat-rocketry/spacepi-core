#ifndef SPACEPI_TARGET_DEPLOYKEYSETUP_HPP
#define SPACEPI_TARGET_DEPLOYKEYSETUP_HPP

#include <chrono>
#include <SpacePi.hpp>
#include <string>
namespace spacepi {
    namespace target {
        namespace deployKey {
            class DeployKeySetup: public spacepi::util::CommandConfigurable {

            public:
                DeployKeySetup(spacepi::util::Command &cmd) noexcept;

                void runCommand();

            private:
                std::string filepath;
            };
        }
    }
}

#endif
