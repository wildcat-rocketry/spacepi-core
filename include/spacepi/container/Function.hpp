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
                constexpr Function() noexcept
                    : type(TNull) {
                }

                /**
                 * \brief Constructs a null Function
                 *
                 * \param[in] null \c nullptr
                 */
                constexpr Function(decltype(nullptr) null) noexcept
                    : type(TNull) {
                }

                /**
                 * \brief Constructs a Function from a function pointer
                 *
                 * \param[in] func The function pointer
                 */
                constexpr Function(Return (*func)(Args...)) noexcept
                    : type(TFunction), data(func) {
                }

                /**
                 * \brief Constructs a Function from a functoid reference
                 *
                 * \param[in] functoid The functoid reference (must remain valid for lifetime of object)
                 * \tparam Functoid The type of functoid
                 */
                template <typename Functoid>
                constexpr Function(Functoid &functoid) noexcept
                    : type(TFunctoid), data(functoid) {
                }

                /**
                 * \brief Constructs a Function from a functoid reference
                 *
                 * \param[in] constFunctoid The functoid reference (must remain valid for lifetime of object)
                 * \tparam Functoid The type of functoid
                 */
                template <typename Functoid>
                constexpr Function(const Functoid &constFunctoid) noexcept
                    : type(TConstFunctoid), data(constFunctoid) {
                }

                /**
                 * \brief Constructs a Function from a class member function
                 *
                 * \param[in] cls The class pointer (must remain valid for lifetime of object)
                 * \tparam Class The class in which the member exists
                 * \tparam Func The member function to call
                 * \return The Function
                 */
                template <typename Class, Return (Class::*Func)(Args...)>
                static constexpr Function create(Class *cls) noexcept {
                    return Function(cls, ClassFunctionTag<Class, Func>());
                }

                /**
                 * \brief Constructs a Function from a class member function
                 *
                 * \param[in] cls The class pointer (must remain valid for lifetime of object)
                 * \tparam Class The class in which the member exists
                 * \tparam Func The member function to call
                 * \return The Function
                 */
                template <typename Class, Return (Class::*Func)(Args...) const>
                static constexpr Function create(const Class *cls) noexcept {
                    return Function(cls, ConstClassFunctionTag<Class, Func>());
                }

                /**
                 * \brief Assigns the Function to null
                 *
                 * \param[in] null \c nullptr
                 * \return \c *this
                 */
                constexpr Function<Return(Args...)> &operator=(decltype(nullptr) null) noexcept {
                    return *new (this) Function<Return(Args...)>();
                }

                /**
                 * \brief Assigns the Function to a function pointer
                 *
                 * \param[in] func The function pointer
                 * \return \c *this
                 */
                constexpr Function<Return(Args...)> &operator=(Return (*func)(Args...)) noexcept {
                    return *new (this) Function<Return(Args...)>(func);
                }

                /**
                 * \brief Assigns the Function to a functoid reference
                 *
                 * \param[in] functoid The functoid reference (must remain valid for lifetime of object)
                 * \return \c *this
                 */
                template <typename Functoid>
                constexpr Function<Return(Args...)> &operator=(Functoid &functoid) noexcept {
                    return *new (this) Function<Return(Args...)>(functoid);
                }

                /**
                 * \brief Assigns the Function to a functoid reference
                 *
                 * \param[in] constFunctoid The functoid reference (must remain valid for lifetime of object)
                 * \return \c *this
                 */
                template <typename Functoid>
                constexpr Function<Return(Args...)> &operator=(const Functoid &constFunctoid) noexcept {
                    return *new (this) Function<Return(Args...)>(constFunctoid);
                }

                /**
                 * \brief Calls the Function
                 *
                 * \param[in] args The function arguments
                 * \return The function return value
                 */
                constexpr Return operator()(Args... args) {
                    return data(type, Parameter::forward<Args>(args)...);
                }

                /**
                 * \brief Calls the Function
                 *
                 * \param[in] args The function arguments
                 * \return The function return value
                 */
                constexpr Return operator()(Args... args) const {
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
                enum FuncType
                {
                    TNull,
                    TFunction,
                    TFunctoid,
                    TConstFunctoid,
                    TClassFunction,
                    TConstClassFunction
                };

                class FunctoidData {
                    public:
                        template <typename Functoid>
                        constexpr FunctoidData(Functoid &functoid) noexcept
                            : functoid(&functoid), wrapper(&wrapperImpl<Functoid>) {
                        }

                        constexpr Return operator()(Args... args) {
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
                        constexpr ConstFunctoidData(const Functoid &functoid) noexcept
                            : functoid(&functoid), wrapper(&wrapperImpl<Functoid>) {
                        }

                        constexpr Return operator()(Args... args) const {
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

                template <typename Class, Return (Class::*Func)(Args...)>
                class ClassFunctionTag {
                };

                class ClassFunctionData {
                    public:
                        template <typename Class, Return (Class::*Func)(Args...)>
                        constexpr ClassFunctionData(Class *cls, ClassFunctionTag<Class, Func> tag) noexcept
                            : cls(cls), wrapper(&wrapperImpl<Class, Func>) {
                        }

                        constexpr Return operator()(Args... args) {
                            return wrapper(cls, Parameter::forward<Args>(args)...);
                        }

                    private:
                        template <typename Class, Return (Class::*Func)(Args...)>
                        static Return wrapperImpl(void *cls, Args... args) {
                            return ((*((Class *) cls)).*Func)(Parameter::forward<Args>(args)...);
                        }

                        void *cls;
                        Return (*wrapper)(void *, Args...);
                };

                template <typename Class, Return (Class::*Func)(Args...) const>
                class ConstClassFunctionTag {
                };

                class ConstClassFunctionData {
                    public:
                        template <typename Class, Return (Class::*Func)(Args...) const>
                        constexpr ConstClassFunctionData(const Class *cls, ConstClassFunctionTag<Class, Func> tag) noexcept
                            : cls(cls), wrapper(&wrapperImpl<Class, Func>) {
                        }

                        constexpr Return operator()(Args... args) const {
                            return wrapper(cls, Parameter::forward<Args>(args)...);
                        }

                    private:
                        template <typename Class, Return (Class::*Func)(Args...) const>
                        static Return wrapperImpl(const void *cls, Args... args) {
                            return ((*((const Class *) cls)).*Func)(Parameter::forward<Args>(args)...);
                        }

                        const void *cls;
                        Return (*wrapper)(const void *, Args...);
                };

                union Data {
                    public:
                        constexpr Data() noexcept {
                        }

                        constexpr Data(Return (*function)(Args...)) noexcept
                            : function(function) {
                        }

                        template <typename Functoid>
                        constexpr Data(Functoid &functoid) noexcept
                            : functoid(functoid) {
                        }

                        template <typename Functoid>
                        constexpr Data(const Functoid &constFunctoid) noexcept
                            : constFunctoid(constFunctoid) {
                        }

                        template <typename Class, Return (Class::*Func)(Args...)>
                        constexpr Data(Class *cls, ClassFunctionTag<Class, Func> tag) noexcept
                            : classFunction(cls, tag) {
                        }

                        template <typename Class, Return (Class::*Func)(Args...) const>
                        constexpr Data(const Class *cls, ConstClassFunctionTag<Class, Func> tag) noexcept
                            : constClassFunction(cls, tag) {
                        }

                        constexpr Return operator()(FuncType type, Args... args) {
                            switch (type) {
                                case TFunction:
                                    return function(Parameter::forward<Args>(args)...);
                                case TFunctoid:
                                    return functoid(Parameter::forward<Args>(args)...);
                                case TConstFunctoid:
                                    return constFunctoid(Parameter::forward<Args>(args)...);
                                case TClassFunction:
                                    return classFunction(Parameter::forward<Args>(args)...);
                                case TConstClassFunction:
                                    return constClassFunction(Parameter::forward<Args>(args)...);
                            }
                            throw 0;
                        }

                        constexpr Return operator()(FuncType type, Args... args) const {
                            switch (type) {
                                case TFunction:
                                    return function(Parameter::forward<Args>(args)...);
                                case TConstFunctoid:
                                    return constFunctoid(Parameter::forward<Args>(args)...);
                                case TConstClassFunction:
                                    return constClassFunction(Parameter::forward<Args>(args)...);
                            }
                            throw 0;
                        }

                    private:
                        Return (*function)(Args...);
                        FunctoidData functoid;
                        ConstFunctoidData constFunctoid;
                        ClassFunctionData classFunction;
                        ConstClassFunctionData constClassFunction;
                };

                template <typename Class, Return (Class::*Func)(Args...)>
                constexpr Function(Class *cls, ClassFunctionTag<Class, Func> tag)
                    : type(TClassFunction), data(cls, tag) {
                }

                template <typename Class, Return (Class::*Func)(Args...) const>
                constexpr Function(const Class *cls, ConstClassFunctionTag<Class, Func> tag)
                    : type(TConstClassFunction), data(cls, tag) {
                }

                FuncType type;
                Data data;
        };
    }
}

#endif
