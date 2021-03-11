#ifndef SPACEPI_CORE_RESOURCE_STREAM_HPP
#define SPACEPI_CORE_RESOURCE_STREAM_HPP

#include <ios>
#include <istream>
#include <streambuf>

namespace spacepi {
    namespace resource {
        /**
         * \brief Base class for resources which are implemented as streams
         */
        class Stream : public std::iostream, public std::streambuf {
            public:
                /**
                 * \brief Initialize the Stream
                 */
                Stream();

                /**
                 * \brief Copy-construct the Stream
                 * 
                 * \param[in] copy The original object
                 */
                Stream(const Stream &copy);

                /**
                 * \brief Copy-assign the Stream
                 * 
                 * \param[in] copy The original object
                 * 
                 * \return \c this
                 */
                Stream &operator =(const Stream &copy);

                /**
                 * \brief Destroys this Stream
                 */
                virtual ~Stream() = default;

            protected:
                /**
                 * \brief Read some data from the Stream
                 * 
                 * \param[out] buffer The data which was read
                 * \param[in] count The maximum amount of data to read
                 * 
                 * \return The actual amount of data which was read
                 */
                std::streamsize xsgetn(char *buffer, std::streamsize count);

                /**
                 * \brief Write some data to the Stream
                 * 
                 * \param[in] buffer The data to write
                 * \param[in] count The maximum amount of data to write
                 * 
                 * \return The actual amount of data which was written
                 */
                std::streamsize xsputn(char *buffer, std::streamsize count);

                /**
                 * \brief Peek the next character to read but don't remove
                 * 
                 * \return The next character to read or EOL
                 */
                int underflow();

                /**
                 * \brief Take the next character off the Stream
                 * 
                 * \return The next character on the Stream or EOL
                 */
                int uflow();

            private:
                /**
                 * \copydoc xsgetn
                 */
                virtual int readBuf(char *buffer, int count) = 0;

                /**
                 * \copydoc xsputn
                 */
                virtual int writeBuf(const char *buffer, int count) = 0;

                int last;
        };
    }
}

#endif
