#include <array>
#include <cstddef>
#include <ios>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>
#include <spacepi/protoc/CodeStream.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::protoc;
using namespace spacepi::protoc::detail;

array<char, 81> CodeStream::indentBuf;
Indenter protoc::indent(1, 1);
Indenter protoc::deindent(1, -1);

CodeStream::CodeStream() noexcept
    : ostream(&buf), indentChars(0), emptyLine(true) {
    static bool init = true;
    if (init) {
        init = false;
        indentBuf.fill(' ');
    }
}

CodeStream::CodeStream(const CodeStream &copy) noexcept
    : ostream(&buf), indentChars(copy.indentChars), emptyLine(copy.emptyLine) {
    buf.str(copy.buf.str());
}

CodeStream &CodeStream::operator=(const CodeStream &copy) noexcept {
    buf.str(copy.buf.str());
    indentChars = copy.indentChars;
    emptyLine = copy.emptyLine;
    return *this;
}

CodeStream &CodeStream::indent(int tabs) noexcept {
    indentChars += 4 * tabs;
    if (indentChars < 0) {
        indentChars = 0;
    }
    return *this;
}

CodeStream &CodeStream::deindent(int tabs) noexcept {
    return indent(-tabs);
}

CodeStream &CodeStream::operator<<(ostream &(*pf)(ostream &) ) noexcept {
    ostringstream ss;
    ss.flags(flags);
    return process((ostringstream &) (ss << pf));
}

CodeStream &CodeStream::operator<<(ios &(*pf)(ios &) ) noexcept {
    ostringstream ss;
    ss.flags(flags);
    return process((ostringstream &) (ss << pf));
}

CodeStream &CodeStream::operator<<(ios_base &(*pf)(ios_base &) ) noexcept {
    ostringstream ss;
    ss.flags(flags);
    return process((ostringstream &) (ss << pf));
}

string CodeStream::str() const noexcept {
    return buf.str();
}

CodeStream &CodeStream::process(const ostringstream &ss) noexcept {
    string str = ss.str();
    size_t start = 0;
    while (start < str.size()) {
        size_t lineStart = start;
        size_t lineEnd = str.find_first_of('\n', start);
        if (lineEnd == string::npos) {
            lineEnd = str.size() + 1;
            start = str.size();
        } else {
            start = lineEnd + 1;
            if (lineEnd > 0 && str[lineEnd - 1] == '\r') {
                --lineEnd;
            }
        }
        if (lineStart < lineEnd) {
            int indentChars = this->indentChars;
            if (indentChars >= indentBuf.size()) {
                indentChars = (int) (indentBuf.size() - 1);
            }
            if (!emptyLine) {
                indentChars = 0;
            }
            write(indentBuf.data(), indentChars);
            write(str.data() + lineStart, lineEnd - lineStart - 1);
        }
        if (emptyLine = (lineEnd != str.size() + 1)) {
            put('\n');
        }
    }
    flags = ss.flags();
    return *this;
}

Indenter::Indenter(int tabs, int sign) noexcept
    : tabs(tabs), sign(sign) {
}

CodeStream &Indenter::operator()(CodeStream &os) const noexcept {
    return os.indent(tabs * sign);
}

Indenter Indenter::operator()(int tabs) const noexcept {
    return Indenter(tabs, sign);
}

CodeStream &protoc::operator<<(CodeStream &os, const Indenter &indenter) noexcept {
    return indenter(os);
}
