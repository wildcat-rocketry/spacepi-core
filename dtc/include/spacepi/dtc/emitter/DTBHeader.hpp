#ifndef SPACEPI_CORE_DTC_EMITTER_DTBHeader_HPP
#define SPACEPI_CORE_DTC_EMITTER_DTBHeader_HPP

#include <map>
#include <ostream>
#include <string>

#include <spacepi/dtc/emitter/DTBFile.hpp>

namespace spacepi {
    namespace dtc {
        namespace emitter {
            /**
             * \brief Header model for the flattened device tree file
             */
            class DTBHeader {
                public:
                    /**
                     * \brief Constructs a LabelMapping
                     */
                    DTBHeader(uint version, uint last_comp) noexcept: version(version), last_comp_version(last_comp) {};
                    DTBHeader() noexcept: version(17), last_comp_version(16) {};

                    void set_totalsize(uint32_t totalsize) noexcept;
                    void set_struct_pos(uint32_t struct_pos) noexcept;
                    void set_strings_pos(uint32_t strings_pos) noexcept;
                    void set_memory_reservations_pos(uint32_t memory_reservations_pos) noexcept;
                    void set_boot_cpuid_phys_pos(uint32_t boot_cpuid_phys_pos) noexcept;
                    void set_strings_size(uint32_t strings_size) noexcept;
                    void set_struct_size(uint32_t struct_size) noexcept;

                    void write_header(emitter::DTBFile &file) const noexcept;

                private:
                    const uint32_t MAGIC = 0xd00dfeed;
                    uint32_t totalsize;
                    uint32_t off_dt_struct;
                    uint32_t off_dt_strings;
                    uint32_t off_mem_rsvmap;
                    const uint32_t version;
                    const uint32_t last_comp_version;
                    uint32_t boot_cpuid_phys;
                    uint32_t size_dt_strings;
                    uint32_t size_dt_struct;
            };
        }
    }
}

#endif

