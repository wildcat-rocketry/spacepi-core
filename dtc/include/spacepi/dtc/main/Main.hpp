#ifndef SPACEPI_CORE_DTC_MAIN_MAIN_HPP
#define SPACEPI_CORE_DTC_MAIN_MAIN_HPP

#include <memory>

namespace spacepi {
    namespace dtc {
        namespace main {
            /**
             * \brief Base class for the main layer logic
             *
             * This layer is responsible for parsing arguments and calling the other layers.
             */
            class Main {
                public:
                    /**
                     * \brief Destroys a Main
                     */
                    virtual ~Main() noexcept = default;

                    /**
                     * \brief Runs the device tree compiler
                     *
                     * \param[in] argc The number of arguments
                     * \param[in] argv The values of the arguments
                     * \return An exit code
                     */
                    virtual int run(int argc, const char **argv) noexcept = 0;

                    /**
                     * \brief The instance of the Main layer
                     */
                    static std::shared_ptr<Main> instance;
            };
        }
    }
}

#endif
