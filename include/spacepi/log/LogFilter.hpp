#ifndef SPACEPI_CORE_LOG_LOGFILTER_HPP
#define SPACEPI_CORE_LOG_LOGFILTER_HPP

#include <string>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/util/Trie.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief Provides functionality to filter which entries are passed on to a LogTarget
         * 
         * Filtering is done based on a map of levels specific to different logging tags, along with a default level
         * which is used when none of the specific tags are matched.
         */
        class LogFilter final {
            public:
                /**
                 * \brief Creates a new LogFilter which passes all entries with a level at least LogLevel::Info
                 */
                LogFilter() noexcept;

                /**
                 * \brief Determines if a log entry should be passed on to a LogTarget
                 * 
                 * \param[in] name The tag of the Logger which created the message
                 * \param[in] level The level at which the message was logged
                 * 
                 * \return If the message should get passed on
                 */
                bool operator ()(const std::string &name, const LogLevel &level) const noexcept;

                /**
                 * \brief Gets the default level to be used if the tag does not match any specific tag
                 * 
                 * \return The level
                 */
                const LogLevel &getDefaultLevel() const noexcept;

                /**
                 * \brief Sets the default level to be used if the tag does not match any specific tag
                 * 
                 * \param[in] level The level
                 */
                void setDefaultLevel(const LogLevel &level) noexcept;

                /**
                 * \brief Determines the minimum level at which messages on a specified Logger should be passed along
                 * 
                 * \param[in] name The tag of the Logger
                 * 
                 * \return The level
                 */
                const LogLevel &getLevel(const std::string &name) const noexcept;

                /**
                 * \brief Sets the minimum level at which messages on a specified Logger should be passed along
                 * 
                 * \param[in] name The tag of the Logger
                 * \param[in] level The level
                 */
                void setLevel(const std::string &name, const LogLevel &level) noexcept;

                /**
                 * \brief Resets a Logger to use the default level instead of a specific level
                 * 
                 * \param[in] name The tag of the Logger
                 */
                void resetLevel(const std::string &name) noexcept;

            private:
                const LogLevel *defaultLevel;
                spacepi::util::Trie<const LogLevel *> levels;
        };
    }
}

#endif
