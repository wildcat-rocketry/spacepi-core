#ifndef SPACEPI_CORE_RESOURCE_MEMORY_HPP
#define SPACEPI_CORE_RESOURCE_MEMORY_HPP

#include <cstdint>
#include <memory>
#include <string>

namespace spacepi {
    namespace resource {
        /**
         * \brief Data class for storing information about memory usage
         */
        class MemoryInfo {
            public:
                /**
                 * \brief Create a new MemoryInfo
                 * 
                 * \param[in] used The number of bytes used
                 * \param[in] total The total number of bytes
                 */
                MemoryInfo(uint64_t used, uint64_t total) noexcept;

                /**
                 * \brief Get the number of bytes used
                 * 
                 * \return The number of bytes used
                 */
                uint64_t bytesUsed() const noexcept;

                /**
                 * \brief Get the number of bytes not used
                 * 
                 * \return The number of bytes not used
                 */
                uint64_t bytesFree() const noexcept;

                /**
                 * \brief Get the total number of bytes
                 * 
                 * \return The total number of bytes
                 */
                uint64_t bytesTotal() const noexcept;

            private:
                uint64_t used;
                uint64_t total;
        };

        /**
         * \brief Data class for storing information about all random-access memories on a system
         */
        class RAMInfo {
            public:
                /**
                 * \brief Create a new RAMInfo
                 * 
                 * \param[in] real The information about physical RAM
                 * \param[in] swap The information about non-volatile space being used as RAM
                 */
                RAMInfo(const MemoryInfo &real, const MemoryInfo &swap) noexcept;

                /**
                 * \brief Gets the information about physical RAM
                 * 
                 * \return The information
                 * 
                 */
                const MemoryInfo &getReal() const noexcept;

                /**
                 * \brief Gets the information about non-volatile space being used as RAM
                 * 
                 * \return The information
                 */
                const MemoryInfo &getSwap() const noexcept;

            private:
                MemoryInfo real;
                MemoryInfo swap;
        };

        /**
         * \brief A hardware resource which determines the availability of different system memories
         */
        class Memory {
            public:
                /**
                 * \brief Get a pointer to a Memory object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the Memory object
                 * 
                 * \return The Memory object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<Memory> get(const std::string &name);

                /**
                 * \brief Get statistics about non-volatile memories
                 * 
                 * \return The statistics
                 */
                virtual MemoryInfo getDiskInfo() = 0;

                /**
                 * \brief Get statistics about volatile memories
                 * 
                 * \return The statistics
                 */
                virtual RAMInfo getRAMInfo() = 0;
        };
    }
}

#endif
