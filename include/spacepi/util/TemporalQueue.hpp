#ifndef SPACEPI_CORE_UTIL_TEMPORALQUEUE_HPP
#define SPACEPI_CORE_UTIL_TEMPORALQUEUE_HPP

#include <utility>

namespace spacepi {
    namespace util {
        template <typename Type, unsigned Size>
        class TemporalQueue {
            public:
                TemporalQueue() : data((Type *) buf), start(0), count(0) {
                }

                TemporalQueue(const TemporalQueue<Type, Size> &copy) : data((Type *) buf), start(0), count(0) {
                    int idx = copy.start;
                    for (int i = 0; i < copy.count; ++i) {
                        push(copy.data[idx]);
                        if (++idx >= Size) {
                            idx = 0;
                        }
                    }
                }

                TemporalQueue(TemporalQueue<Type, Size> &&move) : data((Type *) buf), start(move.start), count(move.count) {
                    while (!move.empty()) {
                        push(std::move(move.front()));
                        move.pop();
                    }
                }

                ~TemporalQueue() {
                    while (!empty()) {
                        pop();
                    }
                    count = 0;
                }

                unsigned size() const {
                    return count;
                }

                unsigned capacity() const {
                    return Size;
                }

                bool empty() const {
                    return count == 0;
                }

                Type &front() {
                    return data[start];
                }

                const Type &front() const {
                    return data[start];
                }

                Type &back() {
                    int idx = start + count - 1;
                    if (idx >= Size) {
                        idx -= Size;
                    }
                    return data[idx];
                }

                const Type &back() const {
                    int idx = start + count - 1;
                    if (idx >= Size) {
                        idx -= Size;
                    }
                    return data[idx];
                }

                void push(const Type &val) {
                    emplace(val);
                }

                void push(Type &&val) {
                    emplace(std::move(val));
                }

                template <typename... Args>
                void emplace(Args &&... args) {
                    if (count == Size) {
                        pop();
                    }
                    ++count;
                    new (&back()) Type(std::forward<Args...>(args...));
                }

                void pop() {
                    front().~Type();
                    if (++start >= Size) {
                        start = 0;
                    }
                    --count;
                }

            private:
                char buf[sizeof(Type) * Size];
                Type *data;
                unsigned start;
                unsigned count;
        };
    }
}

#endif
