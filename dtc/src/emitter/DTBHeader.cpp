#include <spacepi/dtc/emitter/DTBFile.hpp>
#include <spacepi/dtc/emitter/DTBHeader.hpp>

using namespace std;
using namespace spacepi::dtc::emitter;

void DTBHeader::set_totalsize(uint32_t size) noexcept {
    totalsize = size;
}

void DTBHeader::set_struct_pos(uint32_t struct_pos) noexcept {
    struct_pos = struct_pos;
}

void DTBHeader::set_strings_pos(uint32_t strings_pos) noexcept {
    strings_pos = strings_pos;
}

void DTBHeader::set_memory_reservations_pos(uint32_t memory_reservations_pos) noexcept {
    memory_reservations_pos = memory_reservations_pos;
}

void DTBHeader::set_boot_cpuid_phys_pos(uint32_t boot_cpuid_phys_pos) noexcept {
    boot_cpuid_phys_pos = boot_cpuid_phys_pos;
}

void DTBHeader::set_strings_size(uint32_t strings_size) noexcept {
    strings_size = strings_size;
}

void DTBHeader::set_struct_size(uint32_t struct_size) noexcept {
    struct_size = struct_size;
}

void DTBHeader::write_header(emitter::DTBFile &file) const noexcept {
    file.write(MAGIC);
    file.write(totalsize);
    file.write(off_dt_struct);
    file.write(off_dt_strings);
    file.write(off_mem_rsvmap);
    file.write(version);
    file.write(last_comp_version);
    file.write(boot_cpuid_phys);
    file.write(size_dt_strings);
    file.write(size_dt_struct);
}
