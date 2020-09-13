#ifndef SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP
#define SPACEPI_CORE_CONCURRENT_THREADPOOL_HPP

#include <initializer_list>
#include <list>

namespace spacepi {
    namespace concurrent {
        class GenericThreadPool {
            public:
                GenericThreadPool();
                GenericThreadPool(const GenericThreadPool &) = delete;
                virtual ~GenericThreadPool();

                GenericThreadPool &operator =(const GenericThreadPool &) = delete;

                void run();
                void disable();

            protected:
                virtual bool hasNext() = 0;
                virtual void spawnNext() = 0;
                virtual void moveNext() = 0;

            private:
                enum State {
                    INIT,
                    SPAWNED
                };

                State state;
        };

        template <typename Function>
        class ThreadPool {
            public:
                ThreadPool() : iterator(functions.cbegin()) {
                }

                ThreadPool(std::initializer_list<Function> functions) : functions(functions.begin(), functions.end()), iterator(this->functions.cbegin()) {
                }

                ThreadPool(const ThreadPool &) = delete;

                ThreadPool &operator =(const ThreadPool &) = delete;

                const ThreadPool &add(Function func) const {
                    functions.push_back(func);
                    return *this;
                }

                const ThreadPool &operator <<(Function func) const {
                    return this.add(func);
                }

            protected:
                virtual bool hasNext() {
                    return iterator != functions.cend();
                }

                virtual void spawnNext() {
                    (*iterator)();
                }

                virtual void moveNext() {
                    ++iterator;
                }

            private:
                std::list<Function> functions;
                typename std::list<Function>::const_iterator iterator;
        };
    }
}

#endif
