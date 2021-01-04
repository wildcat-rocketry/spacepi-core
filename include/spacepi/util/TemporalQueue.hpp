#ifndef SPACEPI_CORE_UTIL_TEMPORALQUEUE_HPP
#define SPACEPI_CORE_UTIL_TEMPORALQUEUE_HPP

#include <utility>

namespace spacepi {
    namespace util {
        /**
         * \brief A queue of fixed size that only keeps the last \c Size inserted elements
         * 
         * \tparam Type The type of element in the queue
         * \tparam Size The maximum number of elements in the queue
         */
        template <typename Type, unsigned Size>
        class TemporalQueue {
            public:
                /**
                 * \brief Initialize an empty TemporalQueue
                 */
                TemporalQueue() noexcept : data((Type *const) buf), start(0), count(0) {
                }

                /**
                 * \brief Copy-construct a TemporalQueue
                 * 
                 * \param[in] copy The TemporalQueue to copy from
                 */
                TemporalQueue(const TemporalQueue<Type, Size> &copy) : data((Type *const) buf), start(0), count(0) {
                    int idx = copy.start;
                    for (int i = 0; i < copy.count; ++i) {
                        push(copy.data[idx]);
                        if (++idx >= Size) {
                            idx = 0;
                        }
                    }
                }

                /**
                 * \brief Move-construct a TemporalQueue
                 * 
                 * \param[in,out] move The TemporalQueue to move from
                 */
                TemporalQueue(TemporalQueue<Type, Size> &&move) : data((Type *const) buf), start(move.start), count(move.count) {
                    while (!move.empty()) {
                        push(std::move(move.front()));
                        move.pop();
                    }
                }

                /**
                 * \brief Deconstruct all objects contained within the queue
                 */
                ~TemporalQueue() noexcept {
                    while (!empty()) {
                        pop();
                    }
                    count = 0;
                }

                /**
                 * \brief Copy-assign a TemporalQueue
                 * 
                 * \param[in] copy The TemporalQueue to copy from
                 */
                TemporalQueue<Type, Size> &operator =(const TemporalQueue<Type, Size> &copy) {
                    this->~TemporalQueue();
                    new (this) TemporalQueue<Type, Size>(copy);
                    return *this;
                }

                /**
                 * \brief Move-assign a TemporalQueue
                 * 
                 * \param[in,out] move The TemporalQueue to move from
                 */
                TemporalQueue<Type, Size> &operator =(TemporalQueue<Type, Size> &&move) {
                    this->~TemporalQueue();
                    new (this) TemporalQueue<Type, Size>(std::move(move));
                    return *this;
                }

                /**
                 * \brief Get the number of elements stored within the TemporalQueue
                 * 
                 * \return The number of elements
                 */
                unsigned size() const noexcept {
                    return count;
                }

                /**
                 * \brief Get the maximum number of elements which can be stored within the TemporalQueue
                 * 
                 * \return The maximum number of elements
                 */
                unsigned capacity() const noexcept {
                    return Size;
                }

                /**
                 * \brief Determines if the TemporalQueue is empty
                 * 
                 * \return If it is empty
                 */
                bool empty() const noexcept {
                    return count == 0;
                }

                /**
                 * \brief Gets the first (oldest) element in the TemporalQueue
                 * 
                 * \return The first (oldest) element
                 */
                Type &front() noexcept {
                    return data[start];
                }

                /**
                 * \copydoc front()
                 */
                const Type &front() const noexcept {
                    return data[start];
                }

                /**
                 * \brief Gets the last (newest) element in the TemporalQueue
                 * 
                 * \return The last (newest) element
                 */
                Type &back() noexcept {
                    int idx = start + count - 1;
                    if (idx >= Size) {
                        idx -= Size;
                    }
                    return data[idx];
                }

                /**
                 * \copydoc back()
                 */
                const Type &back() const noexcept {
                    int idx = start + count - 1;
                    if (idx >= Size) {
                        idx -= Size;
                    }
                    return data[idx];
                }

                /**
                 * \brief Copy an element into the TemporalQueue
                 * 
                 * \param[in] val The value to copy
                 */
                void push(const Type &val) {
                    emplace(val);
                }

                /**
                 * \brief Move an element into the TemporalQueue
                 * 
                 * \param[in,out] val The value to move
                 */
                void push(Type &&val) {
                    emplace(std::move(val));
                }

                /**
                 * \brief Insert an element into the TemporalQueue by calling the \c Type() constructor
                 * 
                 * \param[in,out] args The arguments to the constructor
                 */
                template <typename... Args>
                void emplace(Args &&... args) {
                    if (count == Size) {
                        pop();
                    }
                    ++count;
                    new (&back()) Type(std::forward<Args...>(args...));
                }

                /**
                 * \brief Remove the first (oldest) element from the TemporalQueue
                 */
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
