#ifndef SPACEPI_CORE_CONTAINER_PROPERTY_HPP
#define SPACEPI_CORE_CONTAINER_PROPERTY_HPP

#include <spacepi/container/AccessMode.hpp>
#include <spacepi/container/Function.hpp>
#include <spacepi/Parameter.hpp>
#include <spacepi/TypeUtil.hpp>

namespace spacepi {
    namespace container {
        /**
         * \private
         */
        template <typename Type, AccessMode Mode = ReadWrite>
        class Property {
            public:
                Property() = delete;
        };

        /**
         * \brief Read-only property syntactic sugar
         *
         * \tparam Type The type of property
         */
        template <typename Type>
        class Property<Type, Read> {
            public:
                /**
                 * \brief Constructs the Property
                 *
                 * \param[in] getter The getter function
                 * \tparam Getter The type of getter function
                 */
                template <typename Getter>
                constexpr Property(Getter &&getter) noexcept : getter(Parameter::forward<Getter &&>(getter)) {
                }

                Property(Property &) = delete;
                Property &operator =(Property &) = delete;

                /**
                 * \brief Gets the value of the property
                 *
                 * \return The property value
                 */
                constexpr operator typename TypeUtil<Type>::PrimitiveOrReference() noexcept {
                    return getter();
                }

                /**
                 * \brief Gets the value of the property
                 *
                 * \return The property value
                 */
                constexpr operator const typename TypeUtil<Type>::PrimitiveOrReference() const noexcept {
                    return getter();
                }

                /**
                 * \brief Gets the value of the property
                 *
                 * \return The property value
                 */
                constexpr typename TypeUtil<Type>::NoRef *operator ->() noexcept {
                    return &getter();
                }

                /**
                 * \brief Gets the value of the property
                 *
                 * \return The property value
                 */
                constexpr const typename TypeUtil<Type>::NoRef *operator ->() const noexcept {
                    return &getter();
                }

            private:
                Function<typename TypeUtil<Type>::PrimitiveOrReference()> getter;
        };

        /**
         * \brief Write-only property syntactic sugar
         *
         * \tparam Type The type of property
         */
        template <typename Type>
        class Property<Type, Write> {
            public:
                /**
                 * \brief Constructs the Property
                 *
                 * \param[in] setter The setter function
                 * \tparam Setter The type of setter function
                 */
                template <typename Setter>
                constexpr Property(Setter &&setter) noexcept : setter(Parameter::forward<Setter &&>(setter)) {
                }

                Property(Property &) = delete;
                Property &operator =(Property &) = delete;

                /**
                 * \brief Sets the value of the property
                 *
                 * \param[in] val The property value
                 * \return \c val
                 */
                constexpr const typename TypeUtil<Type>::PrimitiveOrReference operator =(const typename TypeUtil<Type>::PrimitiveOrReference val) noexcept {
                    setter(val);
                    return val;
                }

            private:
                Function<void(const typename TypeUtil<Type>::PrimitiveOrReference)> setter;
        };

        /**
         * \brief Read and write property syntactix sugar
         *
         * \tparam Type The type of property
         */
        template <typename Type>
        class Property<Type, ReadWrite> : public Property<Type, Read>, public Property<Type, Write> {
            public:
                /**
                 * \brief Constructs the Property
                 *
                 * \param[in] getter The getter function
                 * \param[in] setter The setter function
                 * \tparam Getter The type of getter function
                 * \tparam Setter The type of setter function
                 */
                template <typename Getter, typename Setter>
                constexpr Property(Getter &&getter, Setter &&setter) noexcept : Property<Type, Read>(Parameter::forward<Getter &&>(getter)), Property<Type, Write>(Parameter::forward<Setter &&>(setter)) {
                }

                /**
                 * \brief Sets the value of the property
                 *
                 * \param[in] val The property value
                 * \return \c val
                 */
                constexpr const typename TypeUtil<Type>::PrimitiveOrReference operator =(const typename TypeUtil<Type>::PrimitiveOrReference val) noexcept {
                    return Property<Type, Write>::operator =(val);
                }
        };
    }
}

#endif
