#ifndef SPACEPI_CORE_RESOURCE_MEMORY_HPP
#define SPACEPI_CORE_RESOURCE_MEMORY_HPP

#include <cstdint>
#include <memory>
#include <string>

namespace spacepi {
    namespace resource {
        class MemoryInfo {
            public:
                MemoryInfo(uint64_t used, uint64_t total) noexcept;

                uint64_t bytesUsed() const noexcept;
                uint64_t bytesFree() const noexcept;
                uint64_t bytesTotal() const noexcept;

            private:
                uint64_t used;
                uint64_t total;
        };

        class RAMInfo {
            public:
                RAMInfo(const MemoryInfo &real, const MemoryInfo &swap) noexcept;

                const MemoryInfo &getReal() const noexcept;
                const MemoryInfo &getSwap() const noexcept;

            private:
                MemoryInfo real;
                MemoryInfo swap;
        };

        class Memory {
            public:
                static std::shared_ptr<Memory> get(const std::string &name);

                virtual MemoryInfo getDiskInfo() = 0;
                virtual RAMInfo getRAMInfo() = 0;
        };
    }
}

#endif
