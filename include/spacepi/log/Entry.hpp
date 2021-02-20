#ifndef SPACEPI_CORE_LOG_ENTRY_HPP
#define SPACEPI_CORE_LOG_ENTRY_HPP

#include <chrono>
#include <string>
#include <spacepi/log/LogLevel.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief Contains data about a single logging entry
         * 
         * It is preferred to use the Logger class instead of this class directly.
         */
        class Entry final {
            public:
                /**
                 * \brief Creates an invalid logging entry
                 */
                Entry() noexcept;

                /**
                 * \brief Creates a valid logging entry
                 * 
                 * \param[in] tag The tag of the Logger used to generate the Entry
                 * \param[in] level The level at which the Entry was logged
                 * \param[in] time The time at which the Entry was logged
                 * \param[in] message The message contained inside the Entry
                 */
                Entry(const std::string &tag, const LogLevel &level, const std::chrono::system_clock::time_point &time, const std::string &message) noexcept;

                /**
                 * \brief Gets the tag of the Logger used to generate the Entry
                 * 
                 * \return The tag
                 */
                const std::string &getTag() const noexcept;

                /**
                 * \brief Gets the level at which the Entry was logged
                 * 
                 * \return The level
                 */
                const LogLevel &getLevel() const noexcept;

                /**
                 * \brief Gets the time at which the Entry was logged
                 * 
                 * \return The time
                 */
                const std::chrono::system_clock::time_point &getTime() const noexcept;

                /**
                 * \brief Gets the message contained inside the Entry
                 * 
                 * \return The message
                 */
                const std::string &getMessage() const noexcept;

            private:
                const std::string tag;
                const LogLevel &level;
                const std::chrono::system_clock::time_point time;
                const std::string message;
        };
    }
}

#endif
