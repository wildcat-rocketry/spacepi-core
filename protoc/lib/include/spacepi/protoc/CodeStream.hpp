#ifndef SPACEPI_CORE_PROTOC_CODESTREAM_HPP
#define SPACEPI_CORE_PROTOC_CODESTREAM_HPP

#include <array>
#include <ios>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>

namespace spacepi {
    namespace protoc {
        class CodeStream : private std::ostream {
            public:
                CodeStream() noexcept;
                CodeStream(const CodeStream &copy) noexcept;
                CodeStream &operator =(const CodeStream &copy) noexcept;

                CodeStream &indent(int tabs = 1) noexcept;
                CodeStream &deindent(int tabs = 1) noexcept;

                template <typename Type>
                CodeStream &operator <<(Type val) noexcept {
                    std::ostringstream ss;
                    ss.flags(flags);
                    return process((std::ostringstream &) (ss << val));
                }

                CodeStream &operator <<(std::ostream &(*pf)(std::ostream &)) noexcept;
                CodeStream &operator <<(std::ios &(*pf)(std::ios &)) noexcept;
                CodeStream &operator <<(std::ios_base &(*pf)(std::ios_base &)) noexcept;

                std::string str() const noexcept;

            private:
                CodeStream &process(const std::ostringstream &ss) noexcept;

                static std::array<char, 81> indentBuf;
                std::ios::fmtflags flags;
                std::stringbuf buf;
                int indentChars;
                bool emptyLine;
        };

        namespace detail {
            class Indenter {
                public:
                    Indenter(int tabs, int sign) noexcept;

                    CodeStream &operator ()(CodeStream &os) const noexcept;
                    Indenter operator ()(int tabs = 1) const noexcept;

                private:
                    int tabs;
                    int sign;
            };
        }

        CodeStream &operator <<(CodeStream &os, const detail::Indenter &indenter) noexcept;

        extern detail::Indenter indent;
        extern detail::Indenter deindent;
    }
}

#endif
