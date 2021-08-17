#ifndef SPACEPI_CORE_CONTAINER_WEAKREFERENCE_HPP
#define SPACEPI_CORE_CONTAINER_WEAKREFERENCE_HPP

#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        template <typename Type>
        class StrongReference;

        /**
         * \brief A reference to a StrongReference that is automatically broken when the StrongReference is
         * deconstructed
         *
         * Critical sections using the WeakReference must be kept to a minimum since fast-mode mutexes are used
         * internally.  If longer critical sections are required, consider using SharedPtr instead.
         *
         * \tparam Type The type of object contained in the StrongReference
         * \see StrongReference
         */
        template <typename Type>
        class WeakReference final {
            friend class StrongReference<Type>;

            public:
                /**
                 * \brief Constructs an empty WeakReference
                 */
                constexpr WeakReference() noexcept : next(nullptr), prev(nullptr), val(nullptr) {
                }

                /**
                 * \brief Constructs a WeakReference to a StrongReference
                 *
                 * \param[in,out] ref The StrongReference to which to point
                 */
                inline WeakReference(StrongReference<Type> &ref) noexcept : WeakReference() {
                    *this = ref;
                }

                /**
                 * \brief Copy-constructs the WeakReference
                 *
                 * \param[in,out] copy The WeakReference from which to copy
                 */
                inline WeakReference(WeakReference &copy) noexcept : WeakReference() {
                    *this = copy;
                }

                /**
                 * \brief Move-constructs the WeakReference
                 *
                 * \param[in,out] move The WeakReference from which to move
                 */
                inline WeakReference(WeakReference &&move) noexcept : WeakReference() {
                    *this = Parameter::move<WeakReference>(move);
                }

                /**
                 * \brief Deconstructs the WeakReference
                 */
                inline ~WeakReference() noexcept;

                /**
                 * \brief Assigns the WeakReference to a different StrongReference
                 *
                 * \param[in,out] ref The StrongReference to which to point
                 * \return \c *this
                 */
                inline WeakReference &operator =(StrongReference<Type> &ref) noexcept;

                /**
                 * \brief Copy-assigns the WeakReference
                 *
                 * \param[in,out] copy The WeakReference from which to copy
                 * \return \c *this
                 */
                inline WeakReference &operator =(WeakReference &copy) noexcept;

                /**
                 * \brief Move-assigns the WeakReference
                 *
                 * \param[in,out] move The WeakReference from which to move
                 * \return \c *this
                 */
                inline WeakReference &operator =(WeakReference &&move) noexcept;

                /**
                 * \brief Resolve the WeakReference into a pointer from the StrongReference
                 *
                 * If this method returns non-null, unlock() must be called once the pointer is done being used.
                 *
                 * Critical sections using the WeakReference must be kept to a minimum since fast-mode mutexes are used
                 * internally.  If longer critical sections are required, consider using SharedPtr instead.
                 *
                 * \return The StrongReference's object pointer, or \c nullptr if the reference is stale
                 */
                inline Type *lock() noexcept;

                /**
                 * \brief Resolve the WeakReference into a pointer from the StrongReference
                 *
                 * If this method returns non-null, unlock() must be called once the pointer is done being used.
                 *
                 * Critical sections using the WeakReference must be kept to a minimum since fast-mode mutexes are used
                 * internally.  If longer critical sections are required, consider using SharedPtr instead.
                 *
                 * \return The StrongReference's object pointer, or \c nullptr if the reference is stale
                 */
                inline const Type *lock() const noexcept;

                /**
                 * \brief Marks the end of a critical section started by lock()
                 *
                 * This must only be called exactly once after a call to lock() which returns non-null.
                 */
                inline void unlock() const noexcept;

            private:
                inline void insert() noexcept;
                inline void release() noexcept;

                WeakReference *next;
                WeakReference **prev;
                StrongReference<Type> *val;
        };
    }
}

#include <spacepi/container/ReferenceImpl.hpp>

#endif
