#ifndef SPACEPI_CORE_CONCURRENT_RWMUTEX_HPP
#define SPACEPI_CORE_CONCURRENT_RWMUTEX_HPP

#include <cstdint>
#include <spacepi/concurrent/Semaphore.hpp>

namespace spacepi {
    namespace concurrent {
        template <typename Mutex, typename UniqueLock, typename ConditionVariable>
        class RWMutex : private Semaphore<Mutex, UniqueLock, ConditionVariable> {
            public:
                class ReadSide {
                    friend class RWMutex;

                    public:
                        void lock() {
                            mutex->lockRead();
                        }

                        void unlock() {
                            mutex->unlockRead();
                        }

                    private:
                        ReadSide(RWMutex<Mutex, UniqueLock, ConditionVariable> *mutex) : mutex(mutex) {
                        }

                        RWMutex<Mutex, UniqueLock, ConditionVariable> *mutex;
                };

                class WriteSide {
                    friend class RWMutex;

                    public:
                        void lock() {
                            mutex->lockWrite();
                        }

                        void unlock() {
                            mutex->unlockWrite();
                        }

                    private:
                        WriteSide(RWMutex<Mutex, UniqueLock, ConditionVariable> *mutex) : mutex(mutex) {
                        }

                        RWMutex<Mutex, UniqueLock, ConditionVariable> *mutex;
                };

                RWMutex() : Semaphore<Mutex, UniqueLock, ConditionVariable>(UINT32_MAX), readSide(this), writeSide(this) {
                }

                RWMutex(const RWMutex<Mutex, UniqueLock, ConditionVariable> &) = delete;

                RWMutex<Mutex, UniqueLock, ConditionVariable> &operator =(const RWMutex<Mutex, UniqueLock, ConditionVariable> &) = delete;

                void lockRead() {
                    this->lock(1);
                }

                void lockWrite() {
                    this->lock(UINT32_MAX);
                }

                void unlockRead() {
                    this->unlock(1);
                }

                void unlockWrite() {
                    this->unlock(UINT32_MAX);
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
