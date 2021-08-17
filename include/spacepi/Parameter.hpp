#ifndef SPACEPI_CORE_PARAMETER_HPP
#define SPACEPI_CORE_PARAMETER_HPP

#include <spacepi/TypeUtil.hpp>

namespace spacepi {
    /**
     * \brief Helper functions for handling function parameters
     */
    class Parameter {
        public:
            Parameter() = delete;

            /**
             * \brief Converts an lvalue into an rvalue
             *
             * \param[in] var The lvalue
             * \return The rvalue
             */
            template <typename Target>
            static constexpr typename TypeUtil<Target>::NoRef &&move(typename TypeUtil<Target>::NoRef &var) noexcept {
                return (typename TypeUtil<Target>::NoRef &&) var;
            }

            /**
             * \brief Converts an lvalue into an rvalue
             *
             * \param[in] var The lvalue
             * \return The rvalue
             */
            template <typename Target>
            static constexpr const typename TypeUtil<Target>::NoRef &&move(const typename TypeUtil<Target>::NoRef &var) noexcept {
                return (const typename TypeUtil<Target>::NoRef &&) var;
            }

            /**
             * \brief Converts an lvalue into an rvalue
             *
             * \param[in] var The lvalue
             * \return The rvalue
             */
            template <typename Target>
            static constexpr typename TypeUtil<Target>::NoRef &&move(typename TypeUtil<Target>::NoRef &&var) noexcept {
                return (typename TypeUtil<Target>::NoRef &&) var;
            }

            /**
             * \brief Converts an lvalue into an rvalue
             *
             * \param[in] var The lvalue
             * \return The rvalue
             */
            template <typename Target>
            static constexpr const typename TypeUtil<Target>::NoRef &&move(const typename TypeUtil<Target>::NoRef &&var) noexcept {
                return (const typename TypeUtil<Target>::NoRef &&) var;
            }

            /**
             * \brief Forwards reference information from input to output
             *
             * The template information for this call must always be explicitly specified
             *
             * \param[in] var The modifiable lvalue
             * \return The modifiable lvalue
             */
            template <typename Target>
            static constexpr Target &&forward(typename TypeUtil<Target>::NoRef &var) noexcept {
                return (Target &&) var;
            }

            /**
             * \brief Forwards reference information from input to output
             *
             * The template information for this call must always be explicitly specified
             *
             * \param[in] var The const lvalue
             * \return The const lvalue
             */
            template <typename Target>
            static constexpr const Target &&forward(const typename TypeUtil<Target>::NoRef &var) noexcept {
                return (Target &&) var;
            }

            /**
             * \brief Forwards reference information from input to output
             *
             * The template information for this call must always be explicitly specified
             *
             * \param[in] var The modifiable rvalue
             * \return The modifiable rvalue
             */
            template <typename Target>
            static constexpr Target &&forward(typename TypeUtil<Target>::NoRef &&var) noexcept {
                return var;
            }

            /**
             * \brief Forwards reference information from input to output
             *
             * The template information for this call must always be explicitly specified
             *
             * \param[in] var The const rvalue
             * \return The const rvalue
             */
            template <typename Target>
            static constexpr const Target &&forward(const typename TypeUtil<Target>::NoRef &&var) noexcept {
                return var;
            }
    };
}

#endif
