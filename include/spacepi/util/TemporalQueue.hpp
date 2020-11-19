#ifndef SPACEPI_CORE_UTIL_TEMPORALQUEUE_HPP
#define SPACEPI_CORE_UTIL_TEMPORALQUEUE_HPP

#include <utility>

namespace spacepi {
    namespace util {
        template <typename Type, unsigned Size>
        class TemporalQueue {
            public:
                TemporalQueue() noexcept : data((Type *const) buf), start(0), count(0) {
                }

                TemporalQueue(const TemporalQueue<Type, Size> &copy) : data((Type *const) buf), start(0), count(0) {
                    int idx = copy.start;
                    for (int i = 0; i < copy.count; ++i) {
                        push(copy.data[idx]);
                        if (++idx >= Size) {
                            idx = 0;
                        }
                    }
                }

                TemporalQueue(TemporalQueue<Type, Size> &&move) : data((Type *const) buf), start(move.start), count(move.count) {
                    while (!move.empty()) {
                        push(std::move(move.front()));
                        move.pop();
                    }
                }

                ~TemporalQueue() noexcept {
                    while (!empty()) {
                        pop();
                    }
                    count = 0;
                }

                TemporalQueue<Type, Size> &operator =(TemporalQueue<Type, Size> &copy) {
                    this->~TemporalQueue();
                    new (this) TemporalQueue<Type, Size>(copy);
                    return *this;
                }

                TemporalQueue<Type, Size> &operator =(TemporalQueue<Type, Size> &&move) {
                    this->~TemporalQueue();
                    new (this) TemporalQueue<Type, Size>(std::move(move));
                    return *this;
                }

                unsigned size() const noexcept {
                    return count;
                }

                unsigned capacity() const noexcept {
                    return Size;
                }

                bool empty() const noexcept {
                    return count == 0;
                }

                Type &front() noexcept {
                    return data[start];
                }

                const Type &front() const noexcept {
                    return data[start];
                }

                Type &back() noexcept {
                    int idx = start + count - 1;
                    if (idx >= Size) {
                        idx -= Size;
                    }
                    return data[idx];
                }

                const Type &back() const noexcept {
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

                void pop() noexcept {
                    front().~Type();
                    if (++start >= Size) {
                        start = 0;
                    }
                    --count;
                }

            private:
                char buf[sizeof(Type) * Size];
                Type *const data;
                unsigned start;
                unsigned count;
        };
    }
}

#endif
