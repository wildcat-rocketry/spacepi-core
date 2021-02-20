#ifndef SPACEPI_CORE_LOG_AUTOLOG_HPP
#define SPACEPI_CORE_LOG_AUTOLOG_HPP

#include <string>
#include <utility>
#include <spacepi/log/Logger.hpp>

namespace spacepi {
    namespace log {
        /**
         * \brief Provides a Logger to child classes without any setup
         * 
         * This class is designed such that one can get access to a Logger without needing to do any setup in the
         * constructor of the derived class.
         * 
         * \code{.cpp}
         * #include <spacepi/log/AutoLog.hpp>
         * 
         * class MyClass : private spacepi::log::AutoLog<decltype("myLogger"_autolog)> {
         *     // ...
         * };
         * \endcode
         * 
         * \tparam tag The tag to create the logger with
         */
        template <typename>
        class AutoLog final {
            public:
        };

        template <char... tag>
        class AutoLog<std::integer_sequence<char, tag...>> {
            public:
                /**
                 * \brief Default virtual destructor
                 */
                virtual ~AutoLog() = default;

            protected:
                /**
                 * \brief Creates the logger with the tag given in the class template parameter
                 */
                AutoLog() noexcept : log(getTag()) {
                }

                /**
                 * \brief The logger which has been configured with the given tag
                 */
                Logger log;

            private:
                static constexpr std::string getTag() noexcept {
                    constexpr char str[sizeof...(tag)] = { tag... };
                    return std::string(str, sizeof...(tag));
                }
        };
    }
}

/**
 * \brief Helper method to generate the templating information for spacepi::log::AutoLog
 */
template <typename Char, Char... chars>
constexpr std::integer_sequence<Char, chars...> operator ""_autolog() noexcept {
    return {};
}

#endif
