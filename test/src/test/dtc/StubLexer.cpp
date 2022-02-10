#include <memory>
#include <vector>
#include <spacepi/dtc/lexer/Lexer.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/test/dtc/StubLexer.hpp>

using namespace std;
using namespace spacepi::dtc::lexer;
using namespace spacepi::dtc::tokenizer;
using namespace spacepi::test::dtc;

StubLexer::StubLexer() noexcept
    : old(Lexer::instance) {
    Lexer::instance.reset(new Impl());
}

StubLexer::~StubLexer() noexcept {
    Lexer::instance = old;
}

bool StubLexer::Impl::lex(const vector<Token> &tokens, vector<Lexeme> &lexemes) noexcept {
    for(auto const& token : tokens) {
        lexemes.push_back(Lexeme(Lexeme::StringLiteral, token.getLocation(), token.getValue()));
    }

    return true;
}
