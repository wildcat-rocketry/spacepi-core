#ifndef SPACEPI_CORE_UTIL_SHAREDORREF_HPP
#define SPACEPI_CORE_UTIL_SHAREDORREF_HPP

#include <memory>
#include <type_traits>
#include <utility>
#include <spacepi/util/DisableIf.hpp>

namespace spacepi {
    namespace util {
        template <typename Type>
        class WeakOrRef;

        template <typename Type>
        class SharedOrRef {
            friend class WeakOrRef<Type>;

            public:
                SharedOrRef() : valid(false), ref(nullptr) {
                }

                SharedOrRef(const std::shared_ptr<Type> &ptr) noexcept : valid(ptr), shared(ptr), ref(ptr.get()) {
                }

                template <typename RefType = Type>
                SharedOrRef(typename DisableIf<std::is_base_of<std::enable_shared_from_this<RefType>, RefType>::value, RefType>::type &ref) noexcept : valid(true), ref(&ref) {
                }

                SharedOrRef(std::enable_shared_from_this<Type> &obj) noexcept : valid(true), shared(obj.weak_from_this().lock()), ref(&(Type &) obj) {
                }

                template <typename OtherType>
                SharedOrRef(const SharedOrRef<OtherType> &cast) noexcept : valid(cast.valid), shared(cast.shared), ref(cast.ref) {
                }

                template <typename OtherType>
                SharedOrRef(SharedOrRef<OtherType> &&cast) noexcept : valid(cast.valid), shared(std::move(cast.shared)), ref(cast.ref) {
                }

                SharedOrRef(const SharedOrRef &) = default;
                SharedOrRef &operator =(const SharedOrRef &) = default;
                SharedOrRef(SharedOrRef &&) = default;
                SharedOrRef &operator =(SharedOrRef &&) = default;

                operator bool() const noexcept {
                    return valid;
                }

                bool operator ==(const SharedOrRef &other) const noexcept {
                    if (valid) {
                        return other.valid && ref == other.ref;
                    } else {
                        return !other.valid;
                    }
                }

                bool operator !=(const SharedOrRef &other) const noexcept {
                    if (valid) {
                        return !other.valid || ref != other.ref;
                    } else {
                        return other.valid;
                    }
                }

                Type &operator *() const noexcept {
                    if (!valid) {
                        return *shared;
                    }
                    return *ref;
                }

                Type *operator ->() const noexcept {
                    return ref;
                }

            private:
                bool valid;
                std::shared_ptr<Type> shared;
                Type *ref;
        };
    }
}

#endif
