#ifndef SPACEPI_CORE_DTC_EMITTER_EMITTERIMPL_HPP
#define SPACEPI_CORE_DTC_EMITTER_EMITTERIMPL_HPP

#include <string>

#include <spacepi/dtc/emitter/DTBFile.hpp>
#include <spacepi/dtc/emitter/Emitter.hpp>
#include <spacepi/dtc/emitter/LabelMapping.hpp>
#include <spacepi/dtc/parser/SyntaxProperty.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace emitter {
            /**
             * \brief Implementation of the emitter layer
             */
            class EmitterImpl : public Emitter {
                public:
                    /**
                     * \brief Writes a device tree to an FDT file.
                     *
                     * \param[in] tree The tree to write
                     * \param[in] filename The path to the file to write
                     * \return If it was successful
                     */
                    bool emit(const parser::SyntaxTree &tree, const std::string &filename) const noexcept;
                
                private:
                    const uint32_t FDT_BEGIN_NODE = 0x1;
                    const uint32_t FDT_END_NODE = 0x2;
                    const uint32_t FDT_PROP = 0x3;
                    const uint32_t FDT_NOP = 0x4;
                    const uint32_t FDT_END = 0x9;

                    /**
                     * \brief Writes a device tree propert to the file
                     *
                     * \param[in] file The file to write to
                     * \param[in] labels Label mapping to use
                     * \param[in] base The base tree to find labels in
                     * \param[in] property The property to write
                     */
                    bool write_property(DTBFile &file, LabelMapping &labels, const parser::SyntaxTree &base, const parser::SyntaxProperty &property) const noexcept;

                    /**
                     * \brief Write a node of the tree
                     * 
                     * \param[in] file The file to write to
                     * \param[in] labels Label mapping to use
                     * \param[in] base The base tree to find labels in
                     * \param[in] tree The tree to write 
                     */
                    bool write_node(DTBFile &file, LabelMapping &labels, const parser::SyntaxTree &base, const parser::SyntaxTree &tree) const noexcept;

                    /**
                     * \brief Search the systax tree to find phandle to use
                     * 
                     * \param[in] tree The tree to search
                     * \param[in] name The name of the node to find
                     * 
                     * \return The node with this label
                     */
                    static std::pair<std::string, const parser::SyntaxTree &> find_label(const parser::SyntaxTree &tree, const std::string &label, const std::string &addr = "") noexcept;
            };
        }
    }
}

#endif
