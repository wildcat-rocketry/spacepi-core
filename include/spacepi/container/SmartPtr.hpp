#ifndef SPACEPI_CORE_CONTAINER_SMARTPTR_HPP
#define SPACEPI_CORE_CONTAINER_SMARTPTR_HPP

#include <spacepi/concurrent/Lock.hpp>
#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>
#include <spacepi/container/AllocationType.hpp>
#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        /**
         * \private
         */
        constexpr class ConstructType {
        } Construct;

        /**
         * \brief A pointer to an object which has its lifecycle managed, similar to a limited-functionality garbage
         * collector
         *
         * This only works for garbage collection when no cyclic trees are created.
         *
         * \tparam Type The type of element to manage
         * \tparam Alloc The method to use to allocate the element
         */
        template <typename Type, AllocationType Alloc = Temporary>
        class SmartPtr {
        };

        /**
         * \brief A pointer to an object which has its lifecycle managed, similar to a limited-functionality garbage
         * collector
         *
         * This only works for garbage collection when no cyclic trees are created.
         *
         * The statically-allocated SmartPtr requires the contained object to be movable, as it must be moved from one
         * SmartPtr's buffer to another if the owning SmartPtr is destroyed before all of its members.
         *
         * \tparam Type The type of element to manage
         * \tparam Alloc The method to use to allocate the element
         */
        template <typename Type>
        class SmartPtr<Type, Static> {
            public:
                /**
                 * \brief Constructs a null SmartPtr
                 *
                 * \param[in] null \c nullptr
                 */
                constexpr SmartPtr(decltype(nullptr) null = nullptr) noexcept : state(Null), obj(nullptr), next(this), prev(this) {
                }

                /**
                 * \brief Copy-constructs a SmartPtr
                 *
                 * This new SmartPtr will point to the same object as \c copy.
                 *
                 * \param[in,out] copy The source SmartPtr
                 */
                inline SmartPtr(SmartPtr &copy) noexcept : SmartPtr() {
                    *this = copy;
                }

                /**
                 * \brief Move-constructs a SmartPtr
                 *
                 * This new SmartPtr will point to the same object as \c move, and \c move will no longer point to
                 * anything.
                 *
                 * \param[in,out] move The source SmartPtr
                 */
                inline SmartPtr(SmartPtr &&move) noexcept : SmartPtr() {
                    *this = Parameter::move<SmartPtr>(move);
                }

                /**
                 * \brief Constructs a SmartPtr with a contained element
                 *
                 * \tparam Args... The types of arguments in the element's constructor
                 * \param[in] tag \c Construct
                 * \param[in] args The arguments to the element's constructor
                 */
                template <typename... Args>
                inline SmartPtr(ConstructType tag, Args &&... args) noexcept : SmartPtr() {
                    reset(tag, Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Destroys this SmartPtr
                 *
                 * If another SmartPtr points to the same element, the element is moved to that SmartPtr's buffer.
                 */
                inline ~SmartPtr() noexcept {
                    reset();
                }

                /**
                 * \brief Removes any contained element
                 *
                 * If another SmartPtr points to the same element, the element is moved to that SmartPtr's buffer.
                 *
                 * \param[in] null \c nullptr
                 * \return \c *this
                 */
                inline SmartPtr &operator =(decltype(nullptr) null) noexcept {
                    reset();
                    return *this;
                }

                /**
                 * \brief Copies a SmartPtr
                 *
                 * This new SmartPtr will point to the same object as \c copy.
                 *
                 * \param[in,out] copy The source SmartPtr
                 * \return \c *this
                 */
                inline SmartPtr &operator =(SmartPtr &copy) noexcept {
                    concurrent::Lock<concurrent::Fast> lck(mutex);
                    unsafeReset();
                    if (copy.state != Null) {
                        state = Member;
                        obj = copy.obj;
                        next = copy.next;
                        prev = &copy;
                        next->prev = this;
                        prev->next = this;
                    }
                    return *this;
                }

                /**
                 * \brief Moves a SmartPtr
                 *
                 * This new SmartPtr will point to the same object as \c move, and \c move will no longer point to
                 * anything.
                 *
                 * \param[in,out] move The source SmartPtr
                 * \return \c *this
                 */
                inline SmartPtr &operator =(SmartPtr &&move) noexcept {
                    concurrent::Lock<concurrent::Fast> lck(mutex);
                    unsafeReset();
                    if (move.state != Null) {
                        next = move.next;
                        prev = move.prev;
                        next->prev = this;
                        prev->next = this;
                        if (move.state == Owner) {
                            unsafeMoveOwner(&move, this);
                        }
                    }
                    return *this;
                }

                /**
                 * \brief Removes any contained element
                 *
                 * If another SmartPtr points to the same element, the element is moved to that SmartPtr's buffer.
                 *
                 * \param[in] null \c nullptr
                 */
                inline void reset(decltype(nullptr) null = nullptr) noexcept {
                    concurrent::Lock<concurrent::Fast> lck(mutex);
                    unsafeReset();
                }

                /**
                 * \brief Constructs a SmartPtr with a contained element
                 *
                 * If another SmartPtr points to the same element, the element is moved to that SmartPtr's buffer before
                 * resetting this SmartPtr's buffer.
                 *
                 * \tparam Args... The types of arguments in the element's constructor
                 * \param[in] tag \c Construct
                 * \param[in] args The arguments to the element's constructor
                 */
                template <typename... Args>
                inline void reset(ConstructType tag, Args &&... args) noexcept {
                    concurrent::Lock<concurrent::Fast> lck(mutex);
                    unsafeReset();
                    state = Owner;
                    obj = (Type *) buffer;
                    new (obj) Type(Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator Type *() noexcept {
                    return obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator const Type *() const noexcept {
                    return obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator Type &() noexcept {
                    return *obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator const Type &() const noexcept {
                    return *obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline Type &operator *() noexcept {
                    return *obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline const Type &operator *() const noexcept {
                    return *obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline Type *operator ->() noexcept {
                    return obj;
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline const Type *operator ->() const noexcept {
                    return obj;
                }

                /**
                 * \brief Determines if this SmartPtr has a contained element
                 *
                 * \return If it does
                 */
                inline operator bool() const noexcept {
                    return state != Null;
                }

            private:
                enum State {
                    Null,
                    Owner,
                    Member
                };

                static inline void unsafeMoveOwner(SmartPtr *from, SmartPtr *to) noexcept {
                    to->state = Owner;
                    from->state = Null;
                    new (to->buffer) Type(Parameter::move<Type>(*(Type *) from->buffer));
                    ((Type *) from->buffer)->~Type();
                    to->obj = (Type *) to->buffer;
                    from->obj = nullptr;
                    for (SmartPtr *it = to->next; it != to; it = it->next) {
                        it->obj = to->obj;
                    }
                }

                inline void unsafeReset() noexcept {
                    SmartPtr *newOwner = next;
                    next->prev = prev;
                    prev->next = next;
                    prev = this;
                    next = this;
                    if (state == Owner && newOwner != this) {
                        unsafeMoveOwner(this, newOwner);
                    }
                }

                static concurrent::Mutex<concurrent::Fast> mutex;

                State state;
                char buffer[sizeof(Type)];
                Type *obj;
                SmartPtr *next;
                SmartPtr *prev;
        };

        template <typename Type>
        concurrent::Mutex<concurrent::Fast> SmartPtr<Type, Static>::mutex;
    }
}

#endif
