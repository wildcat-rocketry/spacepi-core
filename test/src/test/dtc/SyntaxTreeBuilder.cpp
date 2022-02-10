#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/test/dtc/SyntaxTreeBuilder.hpp>

using namespace std;
using namespace spacepi::dtc::parser;
using namespace spacepi::test::dtc;

SyntaxTreeBuilder::SyntaxTreeBuilder(SyntaxTree tree) noexcept: tree(tree)
{
}

SyntaxTreeBuilder &SyntaxTreeBuilder::addChild(const SyntaxTree &child) noexcept
{
    tree.addChild(child);
    return *this;
}

SyntaxTreeBuilder &SyntaxTreeBuilder::addInclude(const string &path) noexcept
{
    tree.addInclude(path);
    return *this;
}

SyntaxTreeBuilder &SyntaxTreeBuilder::addProperty(const SyntaxProperty &prop) noexcept
{
    tree.addProperty(prop);
    return *this;
}

SyntaxTreeBuilder &SyntaxTreeBuilder::setPhandle(uint32_t phandle) noexcept
{
    tree.setPhandle(phandle);
    return *this;
}

SyntaxTree SyntaxTreeBuilder::build() noexcept
{
    return tree;
}
