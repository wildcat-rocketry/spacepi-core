#ifndef SPACEPI_CORE_UTIL_WEAKORREF_HPP
#define SPACEPI_CORE_UTIL_WEAKORREF_HPP

#include <memory>
#include <type_traits>
#include <utility>
#include <spacepi/util/SharedOrRef.hpp>

namespace spacepi {
    namespace util {
        /**
         * \brief Pointer which could be either a \c std::weak_ptr or a reference
         * 
         * \tparam Type The type of object to which to point
         */
        template <typename Type>
        class WeakOrRef {
            template <typename OtherType>
            friend class WeakOrRef;

            public:
                /**
                 * \brief Initialize a null WeakOrRef
                 */
                WeakOrRef() noexcept : ref(nullptr) {
                }

                /**
                 * \brief Initialize a WeakOrRef from a \c std::shared_ptr
                 * 
                 * \param[in] ptr The \c std::shared_ptr to initialize from
                 */
                WeakOrRef(const std::shared_ptr<Type> &ptr) noexcept : weak(ptr), ref(nullptr) {
                }

                /**
                 * \brief Initialize a WeakOrRef from a \c std::weak_ptr
                 * 
                 * \param[in] ptr The \c std::weak_ptr to initialize from
                 */
                WeakOrRef(const std::weak_ptr<Type> &ptr) noexcept : weak(ptr), ref(nullptr) {
                }

                /**
                 * \brief Initialize a WeakOrRef from a reference
                 * 
                 * \param[in] ref The reference
                 */
                template <typename RefType = Type>
                WeakOrRef(typename std::enable_if<!std::is_base_of<std::enable_shared_from_this<RefType>, RefType>::value, RefType>::type &ref) noexcept : ref(&ref) {
                }

                /**
                 * \brief Initialize a WeakOrRef from a reference inheriting from \c std::enable_shared_from_this,
                 * converting it to a \c std::weak_ptr first
                 * 
                 * This is equivalent to \c WeakOrRef(obj.weak_from_this())
                 * 
                 * \param[in] obj The object to initialize from
                 */
                WeakOrRef(std::enable_shared_from_this<Type> &obj) noexcept : weak(obj.weak_from_this()), ref(weak.expired() ? (Type *) &obj : nullptr) {
                }

                /**
                 * \brief Initialize a WeakOrRef from a SharedOrRef
                 * 
                 * \param[in] ptr The SharedOrRef to initialize from
                 */
                WeakOrRef(const SharedOrRef<Type> &ptr) noexcept : weak(ptr.shared), ref(ptr.valid && !ptr.shared ? ptr.ref : nullptr) {
                }

                /**
                 * \brief Initialize a WeakOrRef by casting the pointed object from another WeakOrRef
                 * 
                 * \param[in] cast The other WeakOrRef to cast
                 */
                template <typename OtherType>
                WeakOrRef(const WeakOrRef<OtherType> &cast) noexcept : weak(cast.weak), ref(cast.ref) {
                }

                /**
                 * \brief Move-initialize a WeakOrRef by casting the pointed object from another WeakOrRef
                 * 
                 * \param[in,out] cast The other WeakOrRef to cast
                 */
                template <typename OtherType>
                WeakOrRef(WeakOrRef<OtherType> &&cast) noexcept : weak(std::move(cast.weak)), ref(cast.ref) {
                }

                /**
                 * \brief Copy-construct a WeakOrRef
                 * 
                 * \param[in] copy The WeakOrRef to copy from
                 */
                WeakOrRef(const WeakOrRef &copy) = default;

                /**
                 * \brief Copy-assign a WeakOrRef
                 * 
                 * \param[in] copy The WeakOrRef to copy from
                 * 
                 * \return \c this
                 */
                WeakOrRef &operator =(const WeakOrRef &copy) = default;

                /**
                 * \brief Move-construct a WeakOrRef
                 * 
                 * \param[in,out] move The WeakOrRef to move from
                 */
                WeakOrRef(WeakOrRef &&move) = default;

                /**
                 * \brief Move-assign a WeakOrRef
                 * 
                 * \param[in,out] move The WeakOrRef to move from
                 * 
                 * \return \c this
                 */
                WeakOrRef &operator =(WeakOrRef &&move) = default;

                /**
                 * \brief Get a SharedOrRef for this WeakOrRef
                 * 
                 * If this is a reference, it returns a SharedOrRef which contains the same reference.
                 * If this is a \c std::weak_ptr, it returns a SharedOrRef which either contains a \c std::shared_ptr to
                 * the pointed object if it is still valid, otherwise it returns a null SharedOrRef
                 * 
                 * \return The SharedOrRef
                 */
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
