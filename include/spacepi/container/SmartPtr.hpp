#ifndef SPACEPI_CORE_CONTAINER_SMARTPTR_HPP
#define SPACEPI_CORE_CONTAINER_SMARTPTR_HPP

#include <spacepi/concurrent/Lock.hpp>
#include <spacepi/concurrent/Mutex.hpp>
#include <spacepi/concurrent/SleepMode.hpp>
#include <spacepi/container/AccessMode.hpp>
#include <spacepi/container/AllocationType.hpp>
#include <spacepi/container/Function.hpp>
#include <spacepi/container/Property.hpp>
#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        /**
         * \private
         */
        constexpr class ConstructType {
        } Construct;

        /**
         * \private
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
         * The temporary SmartPtr is only suitable for references to either a statically-allocated or
         * dynamically-allocated SmartPtr.
         *
         * \tparam Type The type of element to manage
         * \tparam Alloc The method to use to allocate the element
         */
        template <typename Type>
        class SmartPtr<Type, Temporary> {
            friend class SmartPtr<Type, Static>;

            public:
                /**
                 * \brief Destroys this SmartPtr
                 *
                 * If another SmartPtr points to the same element, the element will not be destroyed.
                 */
                virtual ~SmartPtr() noexcept = default;

                /**
                 * \brief Removes any contained element
                 *
                 * If another SmartPtr points to the same element, the element will not be destroyed.
                 *
                 * \param[in] null \c nullptr
                 * \return \c *this
                 */
                inline SmartPtr &operator =(decltype(nullptr) null) noexcept {
                    reset();
                    return *this;
                }

                /**
                 * \brief Removes any contained element
                 *
                 * If another SmartPtr points to the same element, the element will not be destroyed.
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
                 * If another SmartPtr points to the same element, the element will not be destroyed, and this SmartPtr
                 * will now point to a different element.
                 *
                 * \tparam Args... The types of arguments in the element's constructor
                 * \param[in] tag \c Construct
                 * \param[in] args The arguments to the element's constructor
                 */
                template <typename... Args>
                inline void reset(ConstructType tag, Args &&... args) noexcept {
                    concurrent::Lock<concurrent::Fast> lck(mutex);
                    unsafeReset();
                    unsafeAlloc();
                    new (get()) Type(Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator Type *() noexcept {
                    return get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator const Type *() const noexcept {
                    return get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator Type &() noexcept {
                    return *get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline operator const Type &() const noexcept {
                    return *get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline Type &operator *() noexcept {
                    return *get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline const Type &operator *() const noexcept {
                    return *get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline Type *operator ->() noexcept {
                    return get();
                }

                /**
                 * \brief Gets the element contained within this SmartPtr
                 *
                 * \return The element
                 */
                inline const Type *operator ->() const noexcept {
                    return get();
                }

                /**
                 * \brief Determines if this SmartPtr has a contained element
                 *
                 * \return If it does
                 */
                virtual inline operator bool() const noexcept = 0;

                /**
                 * \brief Determines if this SmartPtr is unique
                 *
                 * If it is unique, the contained element will be destroyed when this object is deleted or reset.
                 */
                const Property<bool, Read> unique;

            protected:
                /**
                 * \private
                 */
                constexpr SmartPtr() noexcept : unique(Function<bool()>::create<SmartPtr, &SmartPtr::isUnique>(this)) {
                }

                /**
                 * \private
                 */
                constexpr SmartPtr(const SmartPtr &copy) noexcept : unique(Function<bool()>::create<SmartPtr, &SmartPtr::isUnique>(this)) {
                }

                /**
                 * \private
                 */
                constexpr SmartPtr(const SmartPtr &&move) noexcept : unique(Function<bool()>::create<SmartPtr, &SmartPtr::isUnique>(this)) {
                }

                /**
                 * \private
                 */
                constexpr SmartPtr &operator =(const SmartPtr &copy) noexcept {
                    return *this;
                }

                /**
                 * \private
                 */
                constexpr SmartPtr &operator =(const SmartPtr &&move) noexcept {
                    return *this;
                }

            private:
                static concurrent::Mutex<concurrent::Fast> mutex;

                virtual void unsafeReset() noexcept = 0;
                virtual void unsafeAlloc() noexcept = 0;
                virtual Type *get() noexcept = 0;
                virtual const Type *get() const noexcept = 0;
                virtual bool isUnique() const noexcept = 0;
        };

        template <typename Type>
        concurrent::Mutex<concurrent::Fast> SmartPtr<Type, Temporary>::mutex;

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
        class SmartPtr<Type, Static> : public virtual SmartPtr<Type, Temporary> {
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
                    SmartPtr<Type, Temporary>::reset(tag, Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Destroys this SmartPtr
                 *
                 * If another SmartPtr points to the same element, the element will not be destroyed.
                 */
                inline ~SmartPtr() noexcept {
                    SmartPtr<Type, Temporary>::reset();
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
                    concurrent::Lock<concurrent::Fast> lck(SmartPtr<Type, Temporary>::mutex);
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
                    concurrent::Lock<concurrent::Fast> lck(SmartPtr<Type, Temporary>::mutex);
                    unsafeReset();
                    if (move.state != Null) {
                        if (move.isUnique()) {
                            next = this;
                            prev = this;
                        } else {
                            next = move.next;
                            prev = move.prev;
                            next->prev = this;
                            prev->next = this;
                        }
                        if (move.state == Owner) {
                            unsafeMoveOwner(&move, this);
                        }
                    }
                    return *this;
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

                inline void unsafeAlloc() noexcept {
                    state = Owner;
                    obj = (Type *) buffer;
                }

                inline Type *get() noexcept {
                    return obj;
                }

                inline const Type *get() const noexcept {
                    return obj;
                }

                inline bool isUnique() const noexcept {
                    return next == prev;
                }

                State state;
                char buffer[sizeof(Type)];
                Type *obj;
                SmartPtr *next;
                SmartPtr *prev;
        };

        /**
         * \brief A pointer to an object which has its lifecycle managed, similar to a limited-functionality garbage
         * collector
         *
         * This only works for garbage collection when no cyclic trees are created.
         *
         * The dynamically-allocated SmartPtr allocates its element on the heap, using \c new and \c delete.
         *
         * \tparam Type The type of element to manage
         * \tparam Alloc The method to use to allocate the element
         */        template <typename Type>
        class SmartPtr<Type, Dynamic> : public virtual SmartPtr<Type, Temporary> {
            public:
                /**
                 * \brief Constructs a null SmartPtr
                 *
                 * \param[in] null \c nullptr
                 */
                constexpr SmartPtr(decltype(nullptr) null = nullptr) noexcept {
                }

                /**
                 * \brief Copy-constructs a SmartPtr
                 *
                 * This new SmartPtr will point to the same object as \c copy.
                 *
                 * \param[in,out] copy The source SmartPtr
                 */
                inline SmartPtr(SmartPtr &copy) noexcept : ptr(copy.ptr) {
                }

                /**
                 * \brief Move-constructs a SmartPtr
                 *
                 * This new SmartPtr will point to the same object as \c move, and \c move will no longer point to
                 * anything.
                 *
                 * \param[in,out] move The source SmartPtr
                 */
                inline SmartPtr(SmartPtr &&move) noexcept : ptr(Parameter::move<SmartPtr<DynamicData, Static>>(move.ptr)) {
                }

                /**
                 * \brief Constructs a SmartPtr with a contained element
                 *
                 * \tparam Args... The types of arguments in the element's constructor
                 * \param[in] tag \c Construct
                 * \param[in] args The arguments to the element's constructor
                 */
                template <typename... Args>
                inline SmartPtr(ConstructType tag, Args &&... args) noexcept {
                    SmartPtr<Type, Temporary>::reset(tag, Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Destroys this SmartPtr
                 *
                 * If another SmartPtr points to the same element, the element will not be destroyed.
                 */
                inline ~SmartPtr() noexcept {
                    SmartPtr<Type, Temporary>::reset();
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
                    ptr = copy.ptr;
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
                    ptr = Parameter::move<SmartPtr<DynamicData, Static>>(move.ptr);
                    return *this;
                }

                /**
                 * \brief Determines if this SmartPtr has a contained element
                 *
                 * \return If it does
                 */
                inline operator bool() const noexcept {
                    return ptr;
                }

            private:
                class DynamicData {
                    public:
                        inline DynamicData() noexcept : data(new char[sizeof(Type)]) {
                        }

                        constexpr DynamicData(DynamicData &&move) noexcept : data(move.data) {
                            move.data = nullptr;
                        }

                        inline ~DynamicData() noexcept {
                            if (data) {
                                delete[] data;
                                data = nullptr;
                            }
                        }

                        constexpr DynamicData &operator =(DynamicData &&move) noexcept {
                            if (data) {
                                delete[] data;
                            }
                            data = move.data;
                            move.data = nullptr;
                            return *this;
                        }

                        inline Type *get() noexcept {
                            return (Type *) data;
                        }

                        inline const Type *get() const noexcept {
                            return (const Type *) data;
                        }

                    private:
                        char *data;
                };

                inline void unsafeReset() noexcept {
                    ptr.reset();
                }

                inline void unsafeAlloc() noexcept {
                    ptr.reset(Construct);
                }

                inline Type *get() noexcept {
                    DynamicData *data = ptr;
                    if (data == nullptr) {
                        return nullptr;
                    }
                    return data->get();
                }

                inline const Type *get() const noexcept {
                    const DynamicData *data = ptr;
                    if (data == nullptr) {
                        return nullptr;
                    }
                    return data->get();
                }

                inline bool isUnique() const noexcept {
                    return !ptr || ptr.unique;
                }

                SmartPtr<DynamicData, Static> ptr;
        };
    }
}

#endif
