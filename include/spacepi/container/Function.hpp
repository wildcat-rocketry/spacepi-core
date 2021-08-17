#ifndef SPACEPI_CORE_CONTAINER_FUNCTION_HPP
#define SPACEPI_CORE_CONTAINER_FUNCTION_HPP

#include <spacepi/Parameter.hpp>

namespace spacepi {
    namespace container {
        /**
         * \private
         */
        template <typename Signature>
        class Function {
            public:
                Function() = delete;
        };

        /**
         * \brief Container which holds a function or functoid
         *
         * \tparam Return The return type
         * \tparam Args... The function argument types
         */
        template <typename Return, typename... Args>
        class Function<Return(Args...)> {
            public:
                /**
                 * \brief Constructs a null Function
                 */
                constexpr Function() noexcept : type(TNull) {
                }

                /**
                 * \brief Constructs a null Function
                 *
                 * \param[in] null \c nullptr
                 */
                constexpr Function(decltype(nullptr) null) noexcept : type(TNull) {
                }

                /**
                 * \brief Constructs a Function from a function pointer
                 *
                 * \param[in] func The function pointer
                 */
                constexpr Function(Return (*func)(Args...)) noexcept : type(TFunction), data(func) {
                }

                /**
                 * \brief Constructs a Function from a functoid reference
                 *
                 * \param[in] functoid The functoid reference (must remain valid for lifetime of object)
                 * \tparam Functoid The type of functoid
                 */
                template <typename Functoid>
                constexpr Function(Functoid &functoid) noexcept : type(TFunctoid), data(functoid) {
                }

                /**
                 * \brief Constructs a Function from a functoid reference
                 *
                 * \param[in] constFunctoid The functoid reference (must remain valid for lifetime of object)
                 * \tparam Functoid The type of functoid
                 */
                template <typename Functoid>
                constexpr Function(const Functoid &constFunctoid) noexcept : type(TConstFunctoid), data(constFunctoid) {
                }

                /**
                 * \brief Assigns the Function to null
                 *
                 * \param[in] null \c nullptr
                 * \return \c *this
                 */
                constexpr Function<Return(Args...)> &operator =(decltype(nullptr) null) noexcept {
                    return *new (this) Function<Return(Args...)>();
                }

                /**
                 * \brief Assigns the Function to a function pointer
                 *
                 * \param[in] func The function pointer
                 * \return \c *this
                 */
                constexpr Function<Return(Args...)> &operator =(Return (*func)(Args...)) noexcept {
                    return *new (this) Function<Return(Args...)>(func);
                }

                /**
                 * \brief Assigns the Function to a functoid reference
                 *
                 * \param[in] functoid The functoid reference (must remain valid for lifetime of object)
                 * \return \c *this
                 */
                template <typename Functoid>
                constexpr Function<Return(Args...)> &operator =(Functoid &functoid) noexcept {
                    return *new (this) Function<Return(Args...)>(functoid);
                }

                /**
                 * \brief Assigns the Function to a functoid reference
                 *
                 * \param[in] constFunctoid The functoid reference (must remain valid for lifetime of object)
                 * \return \c *this
                 */
                template <typename Functoid>
                constexpr Function<Return(Args...)> &operator =(const Functoid &constFunctoid) noexcept {
                    return *new (this) Function<Return(Args...)>(constFunctoid);
                }

                /**
                 * \brief Calls the Function
                 *
                 * \param[in] args The function arguments
                 * \return The function return value
                 */
                constexpr Return operator ()(Args... args) {
                    return data(type, Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Calls the Function
                 *
                 * \param[in] args The function arguments
                 * \return The function return value
                 */
                constexpr Return operator ()(Args... args) const {
                    return data(type, Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Determines if the Function is null
                 *
                 * \return \c false if the Function is null, otherwise \c true
                 */
                constexpr operator bool() const noexcept {
                    return type != TNull;
                }

            private:
                enum FuncType {
                    TNull,
                    TFunction,
                    TFunctoid,
                    TConstFunctoid
                };

                class FunctoidData {
                    public:
                        template <typename Functoid>
                        constexpr FunctoidData(Functoid &functoid) noexcept : functoid(&functoid), wrapper(&wrapperImpl<Functoid>) {
                        }

                        constexpr Return operator ()(Args... args) {
                            return wrapper(functoid, Parameter::forward<Args>(args)...);
                        }

                    private:
                        template <typename Functoid>
                        static Return wrapperImpl(void *functoid, Args... args) {
                            return (*(Functoid *) functoid)(Parameter::forward<Args>(args)...);
                        }

                        void *functoid;
                        Return (*wrapper)(void *, Args...);
                };

                class ConstFunctoidData {
                    public:
                        template <typename Functoid>
                        constexpr ConstFunctoidData(const Functoid &functoid) noexcept : functoid(&functoid), wrapper(&wrapperImpl<Functoid>) {
                        }

                        constexpr Return operator ()(Args... args) const {
                            return wrapper(functoid, Parameter::forward<Args>(args)...);
                        }

                    private:
                        template <typename Functoid>
                        static Return wrapperImpl(const void *functoid, Args... args) {
                            return (*(const Functoid *) functoid)(Parameter::forward<Args>(args)...);
                        }

                        const void *functoid;
                        Return (*wrapper)(const void *, Args...);
                };

                union Data {
                    public:
                        constexpr Data() noexcept {
                        }

                        constexpr Data(Return (*function)(Args...)) noexcept : function(function) {
                        }

                        template <typename Functoid>
                        constexpr Data(Functoid &functoid) noexcept : functoid(functoid) {
                        }

                        template <typename Functoid>
                        constexpr Data(const Functoid &constFunctoid) noexcept : constFunctoid(constFunctoid) {
                        }

                        constexpr Return operator ()(FuncType type, Args... args) {
                            switch (type) {
                                case TFunction:
                                    return function(Parameter::forward<Args>(args)...);
                                case TFunctoid:
                                    return functoid(Parameter::forward<Args>(args)...);
                                case TConstFunctoid:
                                    return constFunctoid(Parameter::forward<Args>(args)...);
                            }
                            throw 0;
                        }

                        constexpr Return operator ()(FuncType type, Args... args) const {
                            switch (type) {
                                case TFunction:
                                    return function(Parameter::forward<Args>(args)...);
                                case TConstFunctoid:
                                    return constFunctoid(Parameter::forward<Args>(args)...);
                            }
                            throw 0;
                        }

                    private:
                        Return (*function)(Args...);
                        FunctoidData functoid;
                        ConstFunctoidData constFunctoid;
                };

                FuncType type;
                Data data;
        };
    }
}

#endif
