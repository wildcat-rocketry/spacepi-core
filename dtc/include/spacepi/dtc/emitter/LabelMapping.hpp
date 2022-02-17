#ifndef SPACEPI_CORE_DTC_EMITTER_LABELMAPPING_HPP
#define SPACEPI_CORE_DTC_EMITTER_LABELMAPPING_HPP

#include <ostream>
#include <string>
#include <unordered_map>

#include <spacepi/dtc/emitter/DTBFile.hpp>

namespace spacepi {
    namespace dtc {
        namespace emitter {
            /**
             * \brief Model class for the string section of a flattened device tree file
             *
             * Each label is stored in the mapping and referenced by property definitions.
             */
            class LabelMapping {
                public:
                    /**
                     * \brief Constructs a LabelMapping
                     *
                     */
                    LabelMapping() noexcept;

                    /**
                     * \brief Add the label to mapping or reuse existing mapping
                     *
                     * \param[in] label The label to add
                     * \return The offset from beginning of strings block for this label
                     */
                    uint32_t add_label(const std::string &label) noexcept;

                    /**
                     * \brief Write the mapping to the file
                     *
                     * \param[in] file The file to write to
                     */
                    void write_labels(emitter::DTBFile &file) const noexcept;

                private:
                    std::unordered_map<std::string, uint32_t> labels;
                    std::size_t strings_pos = 0;
            };
        }
    }
}

#endif
