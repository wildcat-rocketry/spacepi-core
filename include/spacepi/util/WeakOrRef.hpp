#ifndef SPACEPI_CORE_UTIL_WEAKORREF_HPP
#define SPACEPI_CORE_UTIL_WEAKORREF_HPP

#include <memory>
#include <type_traits>
#include <utility>
#include <spacepi/util/DisableIf.hpp>
#include <spacepi/util/SharedOrRef.hpp>

namespace spacepi {
    namespace util {
        template <typename Type>
        class WeakOrRef {
            template <typename OtherType>
            friend class WeakOrRef;

            public:
                WeakOrRef() noexcept : ref(nullptr) {
                }

                WeakOrRef(const std::shared_ptr<Type> &ptr) noexcept : weak(ptr), ref(nullptr) {
                }

                WeakOrRef(const std::weak_ptr<Type> &ptr) noexcept : weak(ptr), ref(nullptr) {
                }

                template <typename RefType = Type>
                WeakOrRef(typename DisableIf<std::is_base_of<std::enable_shared_from_this<RefType>, RefType>::value, RefType>::type &ref) noexcept : ref(&ref) {
                }

                WeakOrRef(std::enable_shared_from_this<Type> &obj) noexcept : weak(obj.weak_from_this()), ref(weak.expired() ? (Type *) &obj : nullptr) {
                }

                WeakOrRef(const SharedOrRef<Type> &ptr) noexcept : weak(ptr.shared), ref(ptr.valid && !ptr.shared ? ptr.ref : nullptr) {
                }

                template <typename OtherType>
                WeakOrRef(const WeakOrRef<OtherType> &cast) noexcept : weak(cast.weak), ref(cast.ref) {
                }

                template <typename OtherType>
                WeakOrRef(WeakOrRef<OtherType> &&cast) noexcept : weak(std::move(cast.weak)), ref(cast.ref) {
                }

                WeakOrRef(const WeakOrRef &) = default;
                WeakOrRef &operator =(const WeakOrRef &) = default;
                WeakOrRef(WeakOrRef &&) = default;
                WeakOrRef &operator =(WeakOrRef &&) = default;

                SharedOrRef<Type> lock() const noexcept {
                    if (ref) {
                        return SharedOrRef<Type>(*ref);
                    } else {
                        return SharedOrRef<Type>(weak.lock());
                    }
                }

            private:
                std::weak_ptr<Type> weak;
                Type *ref;
        };
    }
}

#endif
