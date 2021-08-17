#ifndef SPACEPI_CORE_CONTAINER_REFERENCEIMPL_HPP
#define SPACEPI_CORE_CONTAINER_REFERENCEIMPL_HPP

#include <spacepi/concurrent/Lock.hpp>
#include <spacepi/concurrent/SleepMode.hpp>
#include <spacepi/container/StrongReference.hpp>
#include <spacepi/container/WeakReference.hpp>
#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        template <typename Type>
        inline StrongReference<Type> &StrongReference<Type>::operator =(StrongReference<Type> &&move) noexcept {
            concurrent::Lock<concurrent::Fast> lock(mutex);
            *((Type *) obj) = Parameter::move<Type>(*((Type *) move.obj));
            head = move.head;
            move.head = nullptr;
            for (WeakReference<Type> *ref = head; ref; ref = ref->next) {
                ref->val = this;
            }
        }

        template <typename Type>
        inline StrongReference<Type>::~StrongReference() noexcept {
            concurrent::Lock<concurrent::Fast> lock(mutex);
            for (WeakReference<Type> *ref = head; ref; ref = ref->next) {
                ref->prev = nullptr;
                ref->val = nullptr;
            }
        }

        template <typename Type>
        inline WeakReference<Type>::~WeakReference() noexcept {
            concurrent::Lock<concurrent::Fast> lock(StrongReference<Type>::mutex);
            release();
        }

        template <typename Type>
        inline WeakReference<Type> &WeakReference<Type>::operator =(StrongReference<Type> &ref) noexcept {
            concurrent::Lock<concurrent::Fast> lock(StrongReference<Type>::mutex);
            release();
            val = &ref;
            insert();
            return *this;
        }

        template <typename Type>
        inline WeakReference<Type> &WeakReference<Type>::operator =(WeakReference &copy) noexcept {
            concurrent::Lock<concurrent::Fast> lock(StrongReference<Type>::mutex);
            release();
            val = copy.val;
            if (val) {
                insert();
            }
            return *this;
        }

        template <typename Type>
        inline WeakReference<Type> &WeakReference<Type>::operator =(WeakReference &&move) noexcept {
            concurrent::Lock<concurrent::Fast> lock(StrongReference<Type>::mutex);
            release();
            next = move.next;
            move.next = nullptr;
            prev = move.prev;
            move.prev = nullptr;
            if (prev) {
                *prev = this;
            }
            val = move.val;
            move.val = nullptr;
        }

        template <typename Type>
        inline Type *WeakReference<Type>::lock() noexcept {
            concurrent::Lock<concurrent::Fast> lock(StrongReference<Type>::mutex);
            Type *ret = nullptr;
            if (val) {
                ret = (Type *) val->obj;
                lock.release();
            }
            return ret;
        }

        template <typename Type>
        inline const Type *WeakReference<Type>::lock() const noexcept {
            concurrent::Lock<concurrent::Fast> lock(StrongReference<Type>::mutex);
            const Type *ret = nullptr;
            if (val) {
                ret = (const Type *) val->obj;
                lock.release();
            }
            return ret;
        }

        template <typename Type>
        inline void WeakReference<Type>::unlock() const noexcept {
            StrongReference<Type>::mutex.unlock();
        }

        template <typename Type>
        inline void WeakReference<Type>::insert() noexcept {
            prev = &val->head;
            next = *prev;
            *prev = this;
        }

        template <typename Type>
        inline void WeakReference<Type>::release() noexcept {
            if (next) {
                next->prev = prev;
                next = nullptr;
            }
            if (prev) {
                *prev = next;
                prev = nullptr;
            }
            val = nullptr;
        }
    }
}

#endif
