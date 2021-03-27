#ifndef SPACEPI_CORE_LOG_FILETARGET_HPP
#define SPACEPI_CORE_LOG_FILETARGET_HPP

#include <memory>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogTarget.hpp>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/resource/Stream.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief Logging target which prints the log to a Filesystem resource
         */
        class FileTarget : public LogTarget {
            public:
                /**
                 * \brief Creates a new FileTarget which prints to the given Filesystem resource
                 * 
                 * \param[in,out] fs The file in which to log
                 */
                explicit FileTarget(spacepi::resource::Filesystem &fs) noexcept;

                void operator <<(const Entry &entry);

            private:
                std::shared_ptr<spacepi::resource::Stream> stream;
        };
    }
}

#endif
