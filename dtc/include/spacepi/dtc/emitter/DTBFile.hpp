#ifndef SPACEPI_CORE_DTC_EMITTER_DTBFile_HPP
#define SPACEPI_CORE_DTC_EMITTER_DTBFile_HPP

#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace spacepi {
    namespace dtc {
        namespace emitter {
            /**
             * \brief Device tree blob file
             */
            class DTBFile {
                public:
                    DTBFile(const std::string &filename) noexcept;

                    void write(const std::string &data) noexcept;

                    void write(const uint64_t data) noexcept;
                    void write(const uint32_t data) noexcept;
                    void write(const uint8_t data) noexcept;

                    void write(const std::vector<uint8_t> &data) noexcept;
                    void write(const std::vector<uint32_t> &data) noexcept;
                    void write(const std::vector<uint64_t> &data) noexcept;

                    long position() noexcept;
                    void seek(long pos) noexcept;

                    void close() noexcept;

                private:
                    std::ofstream stream;
            };
        }
    }
}

#endif

