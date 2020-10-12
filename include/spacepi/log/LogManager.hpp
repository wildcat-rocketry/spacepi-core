#ifndef SPACEPI_CORE_LOG_LOGMANAGER_HPP
#define SPACEPI_CORE_LOG_LOGMANAGER_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <spacepi/concurrent/RWMutex.hpp>
#include <spacepi/log/Entry.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogTarget.hpp>

namespace spacepi {
    namespace log {
        class LogManager {
            public:
                LogManager();
                LogManager(const LogManager &) = delete;
                ~LogManager();

                static LogManager instance;

                LogManager &operator =(const LogManager &) = delete;
                void operator <<(const Entry &entry);
                LogManager &operator +=(std::shared_ptr<LogTarget> target);

            private:
                void run();

                std::condition_variable cond;
                std::mutex entryMutex;
                std::queue<Entry> entries;
                std::vector<std::shared_ptr<LogTarget>> targets;
                spacepi::concurrent::RWMutex<std::mutex, std::unique_lock<std::mutex>, std::condition_variable> targetLock;
                bool running;
                std::thread thread;
        };
    }
}

#endif
