#ifndef SPACEPI_CORE_RESOURCE_PROCESSOR_HPP
#define SPACEPI_CORE_RESOURCE_PROCESSOR_HPP

#include <array>
#include <chrono>
#include <memory>
#include <string>

namespace spacepi {
    namespace resource {
        /**
         * \brief Data class for storing information about running processes
         */
        class ProcessInfo final {
            public:
                /**
                 * \brief Create a new ProcessInfo
                 * 
                 * \param[in] numRunning The number of processes currently alive
                 * \param[in] numSleeping The number of processes currently sleeping
                 * \param[in] numStopped The number of processes currently stopped
                 * \param[in] numZombied The number of processes currently zombied
                 */
                ProcessInfo(int numRunning, int numSleeping, int numStopped, int numZombied) noexcept;

                /**
                 * \brief Get the number of processes currently alive
                 * 
                 * \return The number of processes currently alive
                 */
                int countRunning() const noexcept;

                /**
                 * \brief Get the number of processes currently sleeping
                 * 
                 * \return The number of processes currently sleeping
                 */
                int countSleeping() const noexcept;

                /**
                 * \brief Get the number of processes currently stopped
                 * 
                 * \return The number of processes currently stopped
                 */
                int countStopped() const noexcept;

                /**
                 * \brief Get the number of processes currently zombied
                 * 
                 * \return The number of processes currently zombied
                 */
                int countZombied() const noexcept;

            private:
                int numRunning;
                int numSleeping;
                int numStopped;
                int numZombied;
        };

        /**
         * \brief A hardware resource which determines the current processor statistics
         */
        class Processor {
            public:
                /**
                 * \brief Destroys this Processor resource
                 */
                virtual ~Processor() = default;

                /**
                 * \brief Get a pointer to a Processor object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the Processor object
                 * 
                 * \return The Processor object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<Processor> get(const std::string &name);

                /**
                 * \brief Get the number of seconds for which the system has been powered on
                 * 
                 * \return The number of seconds
                 */
                virtual std::chrono::seconds getUptime() = 0;

                /**
                 * \brief Gets the recent load average of the processor
                 * 
                 * The load average is on a scale from 0 to 1, where 0 is no load and 1 is maximum load.
                 * The load average is computed at three different intervals:
                 * \li 1 minute
                 * \li 5 minutes
                 * \li 15 minutes
                 * 
                 * \return The current load averages
                 */
                virtual std::array<double, 3> getLoadAverage() = 0;

                /**
                 * \brief Get the current information about running processes
                 * 
                 * \return The information
                 */
                virtual ProcessInfo getProcessInfo() = 0;
        };
    }
}

#endif
