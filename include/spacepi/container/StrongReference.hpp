#ifndef SPACEPI_CORE_CONTAINER_STRONGREFERENCE_HPP
#define SPACEPI_CORE_CONTAINER_STRONGREFERENCE_HPP

#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>
#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        template <typename Type>
        class WeakReference;

        namespace detail {
            template <typename Type>
            class StrongReference {
                friend class WeakReference<Type>;

                public:
                    constexpr StrongReference() noexcept : head(nullptr) {
                    }

                    virtual Type *lock() noexcept = 0;
                    virtual const Type *lock() const noexcept = 0;
                    virtual void unlock() const noexcept = 0;

                protected:
                    WeakReference<Type> *head;
            };
        }

        /**
         * \brief Contains an object which can be referenced by WeakReference's
         *
         * \tparam Type The type of object
         * \see WeakReference
         */
        template <typename Type>
        class StrongReference final : private detail::StrongReference<Type> {
            friend class WeakReference<Type>;

            public:
                /**
                 * \brief Constructs the StrongReference
                 *
                 * \param[in,out] args The arguments to the Type constructor
                 * \tparam Args... The types of \c args
                 */
                template <typename... Args>
                constexpr StrongReference(Args &&... args) noexcept {
                    new (obj) Type(Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Copy-constructs the StrongReference
                 *
                 * \param[in] copy The StrongReference from which to copy
                 */
                constexpr StrongReference(const StrongReference &copy) noexcept : StrongReference(*((const Type *) copy.obj)) {
                }

                /**
                 * \brief Move-constructs the StrongReference
                 *
                 * \param[in,out] move The StrongReference from which to move
                 */
                inline StrongReference(StrongReference &&move) noexcept {
                    *this = Parameter::move<StrongReference>(move);
                }

                /**
                 * \brief Deconstructs the StrongReference and invalidates any WeakReference's
                 */
                inline ~StrongReference() noexcept;

                /**
                 * \brief Assigns the contained object
                 *
                 * \param[in,out] arg The object to which to assign it
                 * \tparam SetType The type of \c arg
                 */
                template <typename SetType>
                constexpr StrongReference &operator =(SetType &&arg) noexcept {
                    *((Type *) obj) = Parameter::forward<SetType>(arg);
                    return *this;
                }

                /**
                 * \brief Copy-assigns the StrongReference
                 *
                 * \param[in] copy The StrongReference from which to copy
                 * \return \c *this
                 */
                constexpr StrongReference &operator =(const StrongReference &copy) noexcept {
                    *((Type *) obj) = *((const Type *) copy.obj);
                    return *this;
                }

                /**
                 * \brief Move-assigns the StrongReference
                 *
                 * \param[in] move The StrongReference from which to move
                 * \return \c *this
                 */
                inline StrongReference &operator =(StrongReference &&move) noexcept;

                /**
                 * \brief Gets a reference to the contained object
                 *
                 * \return The reference
                 */
                constexpr operator Type &() noexcept {
                    return *(Type *) obj;
                }

                /**
                 * \brief Gets a reference to the contained object
                 *
                 * \return The reference
                 */
                constexpr operator const Type &() const noexcept {
                    return *(const Type *) obj;
                }

                /**
                 * \brief Gets a pointer to the contained object
                 *
                 * \return The pointer
                 */
                constexpr Type *operator ->() noexcept {
                    return (Type *) obj;
                }

                /**
                 * \brief Gets a pointer to the contained object
                 *
                 * \return The pointer
                 */
                constexpr const Type *operator ->() const noexcept {
                    return (const Type *) obj;
                }

            private:
                constexpr Type *lock() noexcept {
                    mutex.lock();
                    return (Type *) obj;
                }

                constexpr const Type *lock() const noexcept {
                    mutex.lock();
                    return (const Type *) obj;
                }

                constexpr void unlock() const noexcept {
                    mutex.unlock();
                }

                static concurrent::Mutex<concurrent::Fast> mutex;

                char obj[sizeof(Type)];
        };

        template <typename Type>
        concurrent::Mutex<concurrent::Fast> StrongReference<Type>::mutex;
    }
}

#include <spacepi/container/ReferenceImpl.hpp>

#endif
