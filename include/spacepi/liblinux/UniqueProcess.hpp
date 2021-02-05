#ifndef SPACEPI_TARGETLIB_LINUX_UNIQUEPROCESS_HPP
#define SPACEPI_TARGETLIB_LINUX_UNIQUEPROCESS_HPP

#include <cstddef>
#include <istream>
#include <memory>
#include <ostream>
#include <queue>
#include <streambuf>
#include <string>
#include <utility>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/system/error_code.hpp>
#include <SpacePi.hpp>

namespace spacepi {
    namespace liblinux {
        namespace detail {
            class OutputStream;

            class OutputStreamCallbacks {
                public:
                    OutputStreamCallbacks(const std::shared_ptr<OutputStream> &os) noexcept;

                    std::pair<boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type>, bool> operator ()(boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> begin, boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> end);
                    void operator ()(const boost::system::error_code &err, size_t count);

                private:
                    std::weak_ptr<OutputStream> os;
            };

            class OutputStream : public std::streambuf, public std::enable_shared_from_this<OutputStream> {
                public:
                    OutputStream(bool use, spacepi::log::Logger &log, spacepi::log::LogLevel level);

                    boost::process::async_pipe &getPipe() noexcept;

                    void handleRead(size_t count);
                    void start();

                protected:
                    std::streamsize showmanyc();
                    std::streamsize xsgetn(char *s, std::streamsize n);
                    int underflow();
                    int uflow();

                private:
                    bool use;
                    spacepi::log::Logger &log;
                    spacepi::log::LogLevel level;
                    boost::process::async_pipe pipe;
                    boost::asio::streambuf buf;
                    std::queue<std::string> readQueue;
                    bool fail;
                    spacepi::concurrent::ConditionVariable cond;
            };

            template <typename Type>
            struct MustBeString {
            };

            template <>
            struct MustBeString<std::string> {
                static inline constexpr const std::string &toString(const std::string &str) {
                    return str;
                }
            };

            template <int Length>
            struct MustBeString<char [Length]> {
                static inline std::string toString(const char *str) {
                    return std::string(str);
                }
            };
        }

        class UniqueProcess {
            public:
                template <typename... Args>
                UniqueProcess(bool useInput, bool useOutput, bool useError, const std::string &exe, const Args &... args) :
                    log(getLogName(exe)),
                    stdoutBuf(new detail::OutputStream(useOutput, log, spacepi::log::LogLevel::Info)), stderrBuf(new detail::OutputStream(useError, log, spacepi::log::LogLevel::Warning)),
                    stdoutStream(stdoutBuf.get()), stderrStream(stderrBuf.get()),
                    proc(exe, detail::MustBeString<Args>::toString(args)..., boost::process::std_in < stdinStream, boost::process::std_out > stdoutBuf->getPipe(), boost::process::std_err > stderrBuf->getPipe()) {
                    init(useInput);
                }

                UniqueProcess(UniqueProcess &) = delete;
                UniqueProcess &operator =(UniqueProcess &) = delete;

                std::ostream &input() noexcept;
                std::istream &output() noexcept;
                std::istream &error() noexcept;

                void closeInput();

                bool running();
                void wait();
                int getExitCode() const;

            private:
                static std::string getLogName(const std::string &exe) noexcept;
                void init(bool useInput);

                spacepi::log::Logger log;
                std::shared_ptr<detail::OutputStream> stdoutBuf;
                std::shared_ptr<detail::OutputStream> stderrBuf;
                boost::process::opstream stdinStream;
                std::istream stdoutStream;
                std::istream stderrStream;
                boost::process::child proc;
        };
    }
}

namespace boost {
    namespace asio {
        template <>
        struct is_match_condition<spacepi::liblinux::detail::OutputStreamCallbacks> : boost::true_type {
        };
    }
}

#endif
