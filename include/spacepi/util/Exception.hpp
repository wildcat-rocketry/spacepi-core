#ifndef SPACEPI_CORE_UTIL_EXCEPTION_HPP
#define SPACEPI_CORE_UTIL_EXCEPTION_HPP

#include <exception>
#include <string>
#include <boost/exception/all.hpp>
#include <boost/format.hpp>
#include <boost/stacktrace.hpp>

namespace spacepi {
    namespace util {
        typedef boost::error_info<struct _stackTraceTag, boost::stacktrace::stacktrace> stackTrace;

        class Exception : public std::exception {
            public:
                typedef boost::exception_ptr pointer;

                Exception(boost::format message);
                Exception(std::string message);

                const char *what() const noexcept;

                template <typename Type>
                static inline pointer createPointer(const Type &ex) {
                    return boost::copy_exception(ex);
                }

                static pointer getPointer();

            private:
                std::string message;
        };

#define SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(name) \
        class name : public Exception { \
            public: \
                name(boost::format message); \
                name(std::string message); \
        };
#include <spacepi/util/Exception.hpp>
#undef SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE
    }
}

std::ostream &operator <<(std::ostream &os, const spacepi::util::Exception::pointer &ex);

#define EXCEPTION(e) boost::enable_error_info(e) \
    << spacepi::util::stackTrace(boost::stacktrace::stacktrace()) \
    << boost::throw_function(BOOST_THROW_EXCEPTION_CURRENT_FUNCTION) \
    << boost::throw_file(__FILE__) \
    << boost::throw_line((int) __LINE__)

#endif
#ifdef SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(StateException)
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(MessagingException)
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(CommandConfigurableException)
#endif
