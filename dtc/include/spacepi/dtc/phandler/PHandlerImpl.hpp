#ifndef SPACEPI_CORE_DTC_PHANDLER_PHANDLERIMPL_HPP
#define SPACEPI_CORE_DTC_PHANDLER_PHANDLERIMPL_HPP

#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/phandler/PHandler.hpp>

namespace spacepi {
    namespace dtc {
        namespace phandler {
            /**
             * \brief Implementaiton of the phandler layer
             */
            class PHandlerImpl : public PHandler {
                public:
                    /**
                     * \brief Assigns default phandle values
                     *
                     * \param[in,out] tree The SyntaxTree to modify
                     */
                    void assign(parser::SyntaxTree &tree) noexcept;
            };
        }
    }
}

#endif
