#ifndef SPACEPI_CORE_UTIL_SHAREDORREF_HPP
#define SPACEPI_CORE_UTIL_SHAREDORREF_HPP

#include <memory>
#include <type_traits>
#include <utility>

namespace spacepi {
    namespace util {
        template <typename Type>
        class WeakOrRef;

        /**
         * \brief Pointer which could be either a \c std::shared_ptr which owns the pointed object, or a reference which
         * does not own the pointed object
         * 
         * \tparam Type The type of object to which to point
         */
        template <typename Type>
        class SharedOrRef {
            friend class WeakOrRef<Type>;

            public:
                /**
                 * \brief Initialize a null SharedOrRef
                 */
                SharedOrRef() : valid(false), ref(nullptr) {
                }

                /**
                 * \brief Initialize a SharedOrRef from a \c std::shared_ptr
                 * 
                 * \param[in] ptr The \c std::shared_ptr to initialize from
                 */
                SharedOrRef(const std::shared_ptr<Type> &ptr) noexcept : valid(ptr), shared(ptr), ref(ptr.get()) {
                }

                /**
                 * \brief Initialize a SharedOrRef from a reference
                 * 
                 * \param[in] ref The reference
                 */
                template <typename RefType = Type>
                SharedOrRef(typename std::enable_if<!std::is_base_of<std::enable_shared_from_this<RefType>, RefType>::value, RefType>::type &ref) noexcept : valid(true), ref(&ref) {
                }

                /**
                 * \brief Initialize a SharedOrRef from a reference inheriting from \c std::enable_shared_from_this,
                 * converting it to a \c std::shared_ptr first
                 * 
                 * This is equivalent to \c SharedOrRef(obj.shared_from_this())
                 * 
                 * \param[in] obj The object to initialize from
                 */
                SharedOrRef(std::enable_shared_from_this<Type> &obj) noexcept : valid(true), shared(obj.weak_from_this().lock()), ref(&(Type &) obj) {
                }

                /**
                 * \brief Initialize a SharedOrRef by casting the pointed object from another SharedOrRef
                 * 
                 * \param[in] cast The other SharedOrRef to cast
                 */
                template <typename OtherType>
                SharedOrRef(const SharedOrRef<OtherType> &cast) noexcept : valid(cast.valid), shared(cast.shared), ref(cast.ref) {
                }

                /**
                 * \brief Move-initialize a SharedOrRef by casting the pointed object from another SharedOrRef
                 * 
                 * \param[in,out] cast The other SharedOrRef to cast
                 */
                template <typename OtherType>
                SharedOrRef(SharedOrRef<OtherType> &&cast) noexcept : valid(cast.valid), shared(std::move(cast.shared)), ref(cast.ref) {
                }

                /**
                 * \brief Copy-construct a SharedOrRef
                 * 
                 * \param[in] copy The SharedOrRef to copy from
                 */
                SharedOrRef(const SharedOrRef &copy) = default;

                /**
                 * \brief Copy-assign a SharedOrRef
                 * 
                 * \param[in] copy The SharedOrRef to copy from
                 * 
                 * \return \c this
                 */
                SharedOrRef &operator =(const SharedOrRef &copy) = default;

                /**
                 * \brief Move-construct a SharedOrRef
                 * 
                 * \param[in,out] move The SharedOrRef to move from
                 */
                SharedOrRef(SharedOrRef &&move) = default;

                /**
                 * \brief Move-assign a SharedOrRef
                 * 
                 * \param[in,out] move The SharedOrRef to move from
                 * 
                 * \return \c this
                 */
                SharedOrRef &operator =(SharedOrRef &&move) = default;

                /**
                 * \brief Determines if this pointer points to a valid object
                 * 
                 * \return If it does
                 */
                operator bool() const noexcept {
                    return valid;
                }

                /**
                 * \brief Compare against another SharedOrRef
                 * 
                 * \param[in] other The RHS of the comparison
                 * 
                 * \return The result of the comparison
                 */
                bool operator ==(const SharedOrRef &other) const noexcept {
                    if (valid) {
                        return other.valid && ref == other.ref;
                    } else {
                        return !other.valid;
                    }
                }

                /**
                 * \copydoc operator==
                 */
                bool operator !=(const SharedOrRef &other) const noexcept {
                    if (valid) {
                        return !other.valid || ref != other.ref;
                    } else {
                        return other.valid;
                    }
                }

                /**
                 * \brief Gets a reference to the object pointed to by this pointer
                 * 
                 * \return The reference
                 */
                Type &operator *() const noexcept {
                    if (!valid) {
                        return *shared;
                    }
                    return *ref;
                }

                /**
                 * \brief Calls a function on the object pointed to by this pointer
                 */
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
