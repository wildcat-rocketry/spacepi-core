#ifndef SPACEPI_CORE_DTC_PARSER_SYNTAXTREE_HPP
#define SPACEPI_CORE_DTC_PARSER_SYNTAXTREE_HPP

#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/parser/SyntaxProperty.hpp>

namespace spacepi {
    namespace dtc {
        namespace parser {
            /**
             * \brief Model class for a node
             */
            class SyntaxTree {
                public:
                    /**
                     * \brief Constructs a SyntaxTree
                     *
                     * \param[in] location The SourceLocation of this node
                     * \param[in] name The name of the node
                     * \param[in] label The label for the node
                     */
                    SyntaxTree(const diagnostics::SourceLocation &location, const std::string &name, const std::string &label = "") noexcept;
                    SyntaxTree(const SyntaxTree &other) noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator==(const SyntaxTree &other) const noexcept;

                    /**
                     * \brief Checks for equality
                     *
                     * \param[in] other The other object
                     * \return The equality result
                     */
                    bool operator!=(const SyntaxTree &other) const noexcept;

                    /**
                     * \brief Gets the SourceLocation of this node
                     *
                     * \return The SourceLocation
                     */
                    const diagnostics::SourceLocation &getLocation() const noexcept;

                    /**
                     * \brief Gets the name of the node
                     *
                     * \return The name
                     */
                    const std::string &getName() const noexcept;

                    /**
                     * \brief Gets the label for the node
                     *
                     * \return The label
                     */
                    const std::string &getLabel() const noexcept;

                    /**
                     * \brief Adds a child node
                     *
                     * \param[in] node The node to add
                     */
                    void addChild(const SyntaxTree &node) noexcept;

                    /**
                     * \brief Gets the children nodes
                     *
                     * \return The nodes
                     */
                    const std::vector<SyntaxTree> &getChildren() const noexcept;

                    /**
                     * \brief Adds a property to this node
                     *
                     * \param[in] prop The property to add
                     */
                    void addProperty(const SyntaxProperty &prop) noexcept;

                    /**
                     * \brief Gets the node's properties
                     *
                     * \return The properties
                     */
                    const std::vector<SyntaxProperty> &getProperties() const noexcept;

                    /**
                     * \brief Adds an include path to this node
                     *
                     * \param[in] path The include file to add
                     */
                    void addInclude(const std::string &path) noexcept;

                    /**
                     * \brief Gets the includes at this node
                     *
                     * \return The includes
                     */
                    const std::vector<std::string> &getIncludes() const noexcept;

                    /**
                     * \brief Sets the phandle for this node
                     *
                     * \param[in] phandle The phandle value
                     */
                    void setPhandle(uint32_t phandle) noexcept;

                    /**
                     * \brief Gets the phandle property for the node
                     *
                     * \return The phandle
                     */
                    uint32_t getPhandle() const noexcept;

                    /**
                     * \brief Merge another node into this one
                     *
                     * \param[in] other The node to merge
                     */
                    void mergeTree(const SyntaxTree &other) noexcept;

                private:
                    diagnostics::SourceLocation location;
                    std::string name;
                    std::string label;
                    std::vector<SyntaxTree> children;
                    std::vector<SyntaxProperty> properties;
                    std::vector<std::string> includes;
            };

            /**
             * \brief Prints a SyntaxTree to a formatted stream for debugging
             *
             * \param[in] os The stream to which to print
             * \param[in] obj The object to print
             * \return \c os
             */
            std::ostream &operator<<(std::ostream &os, const SyntaxTree &obj) noexcept;
        }
    }
}

#endif
