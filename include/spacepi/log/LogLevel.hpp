#ifndef SPACEPI_CORE_LOG_LOGLEVEL_HPP
#define SPACEPI_CORE_LOG_LOGLEVEL_HPP

#include <ostream>
#include <string>

namespace spacepi {
    namespace log {
        /**
         * \brief A level of severity for a given logging entry
         */
        class LogLevel {
            public:
                /**
                 * \brief Register a new LogLevel
                 * 
                 * Each level consists of an ID and a tag.  The tag ends up getting printed in the logs, and the ID is
                 * used to determine how severe the message is.  Higher ID numbers are more severe.
                 * 
                 * \param[in] id The ID of the level to create
                 * \param[in] tag The name of the level to include in logs
                 */
                LogLevel(int id, const std::string &tag) noexcept;

                /**
                 * \brief Compare one LogLevel against another
                 * 
                 * \param[in] other The RHS to compare against
                 * 
                 * \return The result of the comparison
                 */
                bool operator ==(const LogLevel &other) const noexcept;

                /**
                 * \copydoc operator==
                 */
                bool operator !=(const LogLevel &other) const noexcept;

                /**
                 * \copydoc operator==
                 */
                bool operator < (const LogLevel &other) const noexcept;

                /**
                 * \copydoc operator==
                 */
                bool operator <=(const LogLevel &other) const noexcept;

                /**
                 * \copydoc operator==
                 */
                bool operator > (const LogLevel &other) const noexcept;

                /**
                 * \copydoc operator==
                 */
                bool operator >=(const LogLevel &other) const noexcept;

                /**
                 * \brief Gets the tag associated with this LogLevel
                 * 
                 * \return The tag
                 */
                const std::string &getTag() const noexcept;

                /**
                 * \brief The lowest level, good for temporary debugging information
                 */
                static const LogLevel Debug;

                /**
                 * \brief The normal level, good for informative messages about events which are expected to occur
                 */
                static const LogLevel Info;

                /**
                 * \brief The warning level, good for messages about events which were not expected but are okay
                 */
                static const LogLevel Warning;

                /**
                 * \brief The highest level, good for messages about events which cannot be recovered from
                 */
                static const LogLevel Error;

            private:
                const int id;
                const std::string tag;
        };
    }
}

std::ostream &operator <<(std::ostream &os, const spacepi::log::LogLevel &level);

#endif
