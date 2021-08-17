#ifndef SPACEPI_CORE_CONTAINER_BOX_HPP
#define SPACEPI_CORE_CONTAINER_BOX_HPP

#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        /**
         * \private
         */
        constexpr class NoConstructTag {
        } NoConstruct;

        /**
         * \brief Contains zero or one copies of an object
         *
         * \tparam Type The type of object to contain
         */
        template <typename Type>
        class Box final {
            public:
                /**
                 * \brief Constructs a Box with no contained object
                 *
                 * \param[in] tag \c NoConstruct
                 */
                constexpr Box(const NoConstructTag &tag) noexcept : isConstructed(false) {
                }

                /**
                 * \brief Constructs a Box with one contained object
                 *
                 * \param[in,out] args The arguments to the Type constructor
                 * \tparam Args... The types of \c args
                 */
                template <typename... Args>
                constexpr Box(Args &&... args) noexcept : Box(NoConstruct) {
                    construct(Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Copy-constructs a Box
                 *
                 * \param[in,out] copy The Box from which to copy
                 */
                constexpr Box(Box &copy) noexcept : Box(NoConstruct) {
                    if (copy) {
                        construct((Type &) copy);
                    }
                }

                /**
                 * \brief Copy-constructs a Box
                 *
                 * \param[in] copy The Box from which to copy
                 */
                constexpr Box(const Box &copy) noexcept : Box(NoConstruct) {
                    if (copy) {
                        construct((const Type &) copy);
                    }
                }

                /**
                 * \brief Move-constructs a Box
                 *
                 * \param[in,out] move The Box from which to move
                 */
                constexpr Box(Box &&move) noexcept : Box(NoConstruct) {
                    if (move) {
                        construct(Parameter::move<Type>(move));
                    }
                }

                /**
                 * \brief Destructs a Box
                 */
                inline ~Box() noexcept {
                    destruct();
                }

                /**
                 * \brief Assigns the contained object
                 *
                 * \param[in,out] arg The object to which to assign it
                 * \tparam SetType The type of \c arg
                 */
                template <typename SetType>
                constexpr Box &operator =(SetType &&arg) noexcept {
                    if (*this) {
                        ((Type &) *this) = Parameter::forward<SetType>(arg);
                    } else {
                        construct(Parameter::forward<SetType>(arg));
                    }
                    return *this;
                }

                /**
                 * \brief Copy-assigns the Box
                 *
                 * \param[in,out] copy The Box from which to copy
                 * \return \c *this
                 */
                constexpr Box &operator =(Box &copy) noexcept {
                    if (*this) {
                        if (copy) {
                            ((Type &) *this) = (Type &) copy;
                        } else {
                            destruct();
                        }
                    } else if (copy) {
                        construct((Type &) copy);
                    }
                    return *this;
                }

                /**
                 * \brief Copy-assigns the Box
                 *
                 * \param[in] copy The Box from which to copy
                 * \return \c *this
                 */
                constexpr Box &operator =(const Box &copy) noexcept {
                    if (*this) {
                        if (copy) {
                            ((Type &) *this) = (const Type &) copy;
                        } else {
                            destruct();
                        }
                    } else if (copy) {
                        construct((const Type &) copy);
                    }
                    return *this;
                }

                /**
                 * \brief Move-assigns the Box
                 *
                 * \param[in,out] move The Box from which to move
                 * \return \c *this
                 */
                constexpr Box &operator =(Box &&move) noexcept {
                    if (*this) {
                        if (move) {
                            ((Type &) *this) = Parameter::move<Type>(move);
                        } else {
                            destruct();
                        }
                    } else if (move) {
                        construct(Parameter::move<Type>(move));
                    }
                    return *this;
                }

                /**
                 * \brief Determines if this object has a contained object
                 *
                 * \return \c true if it contains an object, \c false otherwise
                 */
                constexpr operator bool() const noexcept {
                    return isConstructed;
                }

                /**
                 * \brief Gets a reference to the contained object
                 *
                 * \return The contained object
                 */
                constexpr operator Type &() noexcept {
                    return *(Type *) buffer;
                }

                /**
                 * \brief Gets a reference to the contained object
                 *
                 * \return The contained object
                 */
                constexpr operator const Type &() const noexcept {
                    return *(const Type *) buffer;
                }

                /**
                 * \brief Gets a reference to the contained object
                 *
                 * \return The contained object
                 */
                constexpr Type &operator *() noexcept {
                    return *(Type *) buffer;
                }

                /**
                 * \brief Gets a reference to the contained object
                 *
                 * \return The contained object
                 */
                constexpr const Type &operator *() const noexcept {
                    return *(const Type *) buffer;
                }

                /**
                 * \brief Gets a pointer to the contained object
                 *
                 * \return The contained object
                 */
                constexpr Type *operator ->() noexcept {
                    return (Type *) buffer;
                }

                /**
                 * \brief Gets a pointer to the contained object
                 *
                 * \return The contained object
                 */
                constexpr const Type *operator ->() const noexcept {
                    return (const Type *) buffer;
                }

                /**
                 * \brief Constructs a new contained object
                 *
                 * \param[in,out] args The arguments to the Type constructor
                 * \tparam Args... The types of \c args
                 */
                template <typename... Args>
                constexpr void construct(Args &&... args) noexcept {
                    destruct();
                    new (buffer) Type(Parameter::forward<Args>(args)...);
                    isConstructed = true;
                }

                /**
                 * \brief Destructs the contained object
                 */
                constexpr void destruct() noexcept {
                    if (isConstructed) {
                        isConstructed = false;
                        (*this)->~Type();
                    }
                }

            private:
                char buffer[sizeof(Type)];
                bool isConstructed;
        };
    }
}

#endif
