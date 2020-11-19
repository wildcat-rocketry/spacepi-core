#ifndef SPACEPI_CORE_LOG_LOGMANAGER_HPP
#define SPACEPI_CORE_LOG_LOGMANAGER_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogTarget.hpp>
#include <spacepi/util/TemporalQueue.hpp>

namespace spacepi {
    namespace log {
        class LogManager {
            public:
                static LogManager instance;

                LogManager();
                ~LogManager();

                LogManager(LogManager &) = delete;
                LogManager &operator =(LogManager &) = delete;

                void operator <<(const Entry &entry);
                LogManager &operator +=(const std::shared_ptr<LogTarget> &target);

            private:
                void run();

                std::condition_variable cond;
                std::mutex entryMutex;
                spacepi::util::TemporalQueue<Entry, 64> entries;
                std::vector<std::shared_ptr<LogTarget>> targets;
                spacepi::concurrent::RWMutex<std::mutex, std::unique_lock<std::mutex>, std::condition_variable> targetLock;
                bool running;
                std::thread thread;
        };
    }
}

#endif
