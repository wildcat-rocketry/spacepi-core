#ifndef SPACEPI_CORE_DTC_MAIN_MAINIMPL_HPP
#define SPACEPI_CORE_DTC_MAIN_MAINIMPL_HPP

#include <memory>
#include <spacepi/dtc/main/Main.hpp>

namespace spacepi {
    namespace dtc {
        namespace main {
            /**
             * \brief Implementation of the main layer
             */
            class MainImpl : public Main {
                public:
                    /**
                     * \brief Runs the device tree compiler
                     *
                     * \param[in] argc The number of arguments
                     * \param[in] argv The values of the arguments
                     * \return An exit code
                     */
                    int run(int argc, const char **argv) noexcept;
            };
        }
    }
}

#endif
