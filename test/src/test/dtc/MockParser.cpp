#include <memory>
#include <unordered_map>
#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/parser/Parser.hpp>
#include <spacepi/test/dtc/MockParser.hpp>

using namespace std;
using namespace spacepi::dtc::lexer;
using namespace spacepi::dtc::parser;
using namespace spacepi::test::dtc;

MockParser::MockParser(const unordered_map<string, SyntaxTree> &tree_mapping) noexcept
    : tree_mapping(tree_mapping), old(Parser::instance)
{
    Parser::instance.reset(new Impl(this));
}

MockParser::~MockParser() noexcept
{
    Parser::instance = old;
}

MockParser::MockParser::Impl::Impl(MockParser *mock) noexcept
    : mock(mock)
{
}

SyntaxTree MockParser::MockParser::Impl::parse(const vector<Lexeme> &lexemes) noexcept
{
    return SyntaxTree(mock->tree_mapping.at(lexemes[0].getStringValue()));
}
