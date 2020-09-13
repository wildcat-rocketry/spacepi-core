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
                Exception(boost::format message);
                Exception(std::string message);

                const char *what() const noexcept;

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

#define EXCEPTION(e) boost::enable_error_info(e) \
    << spacepi::util::stackTrace(boost::stacktrace::stacktrace()) \
    << boost::throw_function(BOOST_THROW_EXCEPTION_CURRENT_FUNCTION) \
    << boost::throw_file(__FILE__) \
    << boost::throw_line((int) __LINE__)

#endif
#ifdef SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(StateException)
#endif
