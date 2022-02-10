#include <memory>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceFile.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>
#include <spacepi/test/dtc/StubTokenizer.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::tokenizer;
using namespace spacepi::test::dtc;

StubTokenizer::StubTokenizer() noexcept : old(Tokenizer::instance) {
    Tokenizer::instance.reset(new Impl());
}

StubTokenizer::~StubTokenizer() noexcept {
    Tokenizer::instance = old;
}

bool StubTokenizer::Impl::readFile(const std::string &filename, std::vector<Token> &tokens) noexcept {
    tokens.push_back(Token(SourceLocation(shared_ptr<SourceFile>(new SourceFile(filename, { filename })), 0, 0, 0), filename));
    return true;
}
