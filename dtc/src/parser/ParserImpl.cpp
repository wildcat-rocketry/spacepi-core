#include <vector>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/parser/Parser.hpp>
#include <spacepi/dtc/parser/ParserImpl.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::lexer;
using namespace spacepi::dtc::parser;

SyntaxTree ParserImpl::parse(const vector<Lexeme> &lexemes) noexcept {
    // TODO
    return SyntaxTree(SourceLocation(), "");
}
