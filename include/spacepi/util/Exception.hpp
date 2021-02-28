#ifndef SPACEPI_CORE_UTIL_EXCEPTION_HPP
#define SPACEPI_CORE_UTIL_EXCEPTION_HPP

#include <exception>
#include <string>
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

namespace spacepi {
    namespace util {
        /**
         * \brief Stacktrace information to be added to an Exception
         */
        typedef boost::error_info<struct _stackTraceTag, boost::stacktrace::stacktrace> stackTrace;

        /**
         * \brief Base class for exceptions with more functionality than \c std::exception
         */
        class Exception : public std::exception {
            public:
                /**
                 * \brief A pointer to an Exception
                 */
                typedef boost::exception_ptr pointer;

                /**
                 * \brief Creates a new Exception with a given message
                 * 
                 * \param[in] message A description of the Exception
                 */
                explicit Exception(const std::string &message) noexcept;

                /**
                 * \brief Destroys this Exception
                 */
                virtual ~Exception() = default;

                /**
                 * \brief Get a description of the Exception
                 * 
                 * \return The description
                 */
                const char *what() const noexcept;

                /**
                 * \brief Create a pointer to an Exception
                 * 
                 * \param[in] ex The Exception to which to create the pointer
                 * 
                 * \return The pointer
                 */
                template <typename Type>
                static inline pointer createPointer(const Type &ex) noexcept {
                    return boost::copy_exception(ex);
                }

                /**
                 * \brief Gets a pointer to the current Exception
                 * 
                 * Only call this method inside a \c catch block
                 * 
                 * \return The pointer
                 */
                static pointer getPointer() noexcept;

                /**
                 * \brief Get a description of an Exception
                 * 
                 * \param[in] ptr A pointer to the Exception
                 * 
                 * \return The description
                 */
                static std::string what(pointer ptr) noexcept;

            private:
                const std::string message;
        };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(name) \
        class name : public Exception { \
            public: \
                explicit name(const std::string &message) noexcept; \
        };
#include <spacepi/util/Exception.hpp>
#undef SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE
#endif
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
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(InterruptException)
SPACEPI_CORE_UTIL_EXCEPTION_INSTANCE(NotImplementedException)
#endif
