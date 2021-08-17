#ifndef SPACEPI_CORE_CONTAINER_SHAREDREFERENCE_HPP
#define SPACEPI_CORE_CONTAINER_SHAREDREFERENCE_HPP

#include <spacepi/container/StrongReference.hpp>
#include <spacepi/container/WeakReference.hpp>
#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        /**
         * \brief Contains an object which can be referenced by WeakReference's or other SharedReference's
         *
         * \tparam Type the type of object
         * \see WeakReference
         */
        template <typename Type>
        class SharedReference final : private detail::StrongReference<Type> {
            friend class WeakReference<Type>;

            public:
                /**
                 * \brief Constructs the SharedReference
                 *
                 * \param[in,out] args The arguments to the Type constructor
                 * \tparam Args... The types of \c args
                 */
                template <typename... Args>
                constexpr SharedReference(Args &&... args) noexcept : strong(Parameter::forward<Args>(args)...), weak(strong) {
                }

                /**
                 * \brief Copy-constructs the SharedReference
                 *
                 * This is similar to creating a WeakReference from a StrongReference
                 *
                 * \param[in] copy The SharedReference from which to copy
                 */
                inline SharedReference(SharedReference &copy) noexcept : weak(copy.weak) {
                }

                /**
                 * \brief Move-constructs the SharedReference
                 *
                 * This is similar to moving a StrongReference
                 *
                 * \param[in,out] move The SharedReference from which to move
                 */
                inline SharedReference(SharedReference &&move) noexcept : strong(Parameter::move<StrongReference<Type>>(move.strong)), weak(Parameter::move<WeakReference<Type>>(strong)) {
                }

                /**
                 * \brief Copy-assigns the SharedReference
                 *
                 * This is similar to creating a WeakReference from a StrongReference
                 *
                 * \param[in] copy The SharedReference from which to copy
                 * \return \c *this
                 */
                inline SharedReference &operator =(SharedReference &copy) noexcept {
                    strong = Type();
                    weak = Parameter::move<WeakReference<Type>>(copy.weak);
                    return *this;
                }

                /**
                 * \brief Move-assigns the SharedReference
                 *
                 * This is similar to moving a StrongReference
                 *
                 * \param[in] move The SharedReference from which to move
                 * \return \c *this
                 */
                inline SharedReference &operator =(SharedReference &&move) noexcept {
                    strong = Parameter::move<StrongReference<Type>>(move.strong);
                    weak = Parameter::move<WeakReference<Type>>(move.weak);
                    return *this;
                }

                /**
                 * \brief Resolve the SharedReference into a pointer
                 *
                 * If this method returns non-null, unlock() must be called once the pointer is done being used.
                 *
                 * Critical sections using the SharedReference must be kept to a minimum since fast-mode mutexes are
                 * used internally.  If longer critical sections are required, consider using SharedPtr instead.
                 *
                 * \return The object pointer, or \c nullptr if the reference is stale
                 */
                inline Type *lock() noexcept {
                    return weak.lock();
                }

                /**
                 * \brief Resolve the SharedReference into a pointer
                 *
                 * If this method returns non-null, unlock() must be called once the pointer is done being used.
                 *
                 * Critical sections using the SharedReference must be kept to a minimum since fast-mode mutexes are
                 * used internally.  If longer critical sections are required, consider using SharedPtr instead.
                 *
                 * \return The object pointer, or \c nullptr if the reference is stale
                 */
                inline const Type *lock() const noexcept {
                    return weak.lock();
                }

                /**
                 * \brief Marks the end of a critical section started by lock()
                 *
                 * This must only be called exactly once after a call to lock() which returns non-null.
                 */
                inline void unlock() const noexcept {
                    return weak.unlock();
                }

            private:
                StrongReference<Type> strong;
                WeakReference<Type> weak;
        };
    }
}

#endif
