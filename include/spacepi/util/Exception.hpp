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

                explicit Exception(const boost::format &message) noexcept;
                explicit Exception(const std::string &message) noexcept;

                const char *what() const noexcept;

                template <typename Type>
                static inline pointer createPointer(const Type &ex) noexcept {
                    return boost::copy_exception(ex);
                }

                static pointer getPointer() noexcept;
                static std::string what(pointer ptr) noexcept;

            private:
                const std::string message;
        };

#define SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(name) \
        class name : public Exception { \
            public: \
                explicit name(const boost::format &message) noexcept; \
                explicit name(const std::string &message) noexcept; \
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
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(SegmentationFaultException)
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(StateException)
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(ArgumentException)
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(ResourceException)
#endif
