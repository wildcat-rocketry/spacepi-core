#ifndef SPACEPI_CORE_DTC_INCLUDER_INCLUDER_HPP
#define SPACEPI_CORE_DTC_INCLUDER_INCLUDER_HPP

#include <memory>
#include <string>
#include <vector>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace includer {
            /**
             * \brief Base class for the includer layer logic
             *
             * This layer is responsible for resolving includes and merging SyntaxTrees together
             */
            class Includer {
                public:
                    /**
                     * \brief Destroys an Includer
                     */
                    virtual ~Includer() noexcept = default;

                    /**
                     * \brief Loads a file and processes all of its includes to produce a merged SyntaxTree
                     *
                     * \param[in] filename The name of the file (relative to an includeDir) which is the main source file
                     * \param[in] includeDirs The directories in which source files may reside
                     * \return The merged SyntaxTree
                     */
                    parser::SyntaxTree process(const std::string &filename, const std::vector<std::string> &includeDirs) noexcept;

                    /**
                     * \brief The instance of the Includer layer
                     */
                    static std::shared_ptr<Includer> instance;

                private:
                    /**
                     * \brief Processes includes on a SyntaxTree
                     *
                     * \param[in] tree The SyntaxTree to process includes on
                     * \param[in] includeDirs The directories in which source files may reside
                     * \return The SyntaxTree with its includes merged
                     */
                    virtual parser::SyntaxTree process(const parser::SyntaxTree &tree, const std::vector<std::string> &includeDirs) noexcept = 0;
            };
        }
    }
}

#endif
