#ifndef SPACEPI_CORE_CONCURRENT_RWMUTEX_HPP
#define SPACEPI_CORE_CONCURRENT_RWMUTEX_HPP

#include <cstdint>
#include <limits>
#include <spacepi/concurrent/Semaphore.hpp>

namespace spacepi {
    namespace concurrent {
        template <typename Mutex, typename UniqueLock, typename ConditionVariable>
        class RWMutex : private Semaphore<Mutex, UniqueLock, ConditionVariable> {
            public:
                class ReadSide {
                    friend class RWMutex;

                    public:
                        ReadSide(ReadSide &) = delete;
                        ReadSide &operator =(ReadSide &) = delete;

                        void lock() {
                            mutex.lockRead();
                        }

                        void unlock() {
                            mutex.unlockRead();
                        }

                    private:
                        explicit ReadSide(RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex) : mutex(mutex) {
                        }

                        RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex;
                };

                class WriteSide {
                    friend class RWMutex;

                    public:
                        WriteSide(WriteSide &) = delete;
                        WriteSide &operator =(WriteSide &) = delete;

                        void lock() {
                            mutex.lockWrite();
                        }

                        void unlock() {
                            mutex.unlockWrite();
                        }

                    private:
                        explicit WriteSide(RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex) : mutex(mutex) {
                        }

                        RWMutex<Mutex, UniqueLock, ConditionVariable> &mutex;
                };

                RWMutex() : Semaphore<Mutex, UniqueLock, ConditionVariable>(std::numeric_limits<uint32_t>::max()), readSide(*this), writeSide(*this) {
                }

                RWMutex(RWMutex<Mutex, UniqueLock, ConditionVariable> &) = delete;
                RWMutex<Mutex, UniqueLock, ConditionVariable> &operator =(RWMutex<Mutex, UniqueLock, ConditionVariable> &) = delete;

                void lockRead() {
                    this->lock(1);
                }

                void lockWrite() {
                    this->lock(std::numeric_limits<uint32_t>::max());
                }

                void unlockRead() {
                    this->unlock(1);
                }

                void unlockWrite() {
                    this->unlock(std::numeric_limits<uint32_t>::max());
                }

                ReadSide &read() {
                    return readSide;
                }

                WriteSide &write() {
                    return writeSide;
                }

            private:
                ReadSide readSide;
                WriteSide writeSide;
        };
    }
}

#endif
