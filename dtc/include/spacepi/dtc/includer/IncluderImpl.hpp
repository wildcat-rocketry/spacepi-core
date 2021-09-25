#ifndef SPACEPI_CORE_DTC_INCLUDER_INCLUDERIMPL_HPP
#define SPACEPI_CORE_DTC_INCLUDER_INCLUDERIMPL_HPP

#include <string>
#include <vector>
#include <spacepi/dtc/includer/Includer.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace includer {
            /**
             * \brief Implementation of the includer layer
             */
            class IncluderImpl : public Includer {
                private:
                    /**
                     * \brief Processes includes on a SyntaxTree
                     *
                     * \param[in] tree The SyntaxTree to process includes on
                     * \param[in] includeDirs The directories in which source files may reside
                     * \return The SyntaxTree with its includes merged
                     */
                    parser::SyntaxTree process(const parser::SyntaxTree &tree, const std::vector<std::string> &includeDirs) noexcept;
            };
        }
    }
}

#endif
