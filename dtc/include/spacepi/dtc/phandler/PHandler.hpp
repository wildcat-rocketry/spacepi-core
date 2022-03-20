#ifndef SPACEPI_CORE_DTC_PHANDLER_PHANDLER_HPP
#define SPACEPI_CORE_DTC_PHANDLER_PHANDLER_HPP

#include <memory>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace phandler {
            /**
             * \brief Base class for the phandler layer logic
             *
             * This layer is responsible for assigning default phandle values
             */
            class PHandler {
                public:
                    /**
                     * \brief Destroys a PHandler
                     */
                    virtual ~PHandler() noexcept = default;

                    /**
                     * \brief Assigns default phandle values
                     *
                     * \param[in,out] tree The SyntaxTree to modify
                     */
                    virtual void assign(parser::SyntaxTree &tree) noexcept = 0;

                    /**
                     * \brief The instance of the PHandler layer
                     */
                    static std::shared_ptr<PHandler> instance;
            };
        }
    }
}

#endif
