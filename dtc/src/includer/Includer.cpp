#include <memory>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/includer/Includer.hpp>
#include <spacepi/dtc/includer/IncluderImpl.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::includer;
using namespace spacepi::dtc::parser;

shared_ptr<Includer> Includer::instance(new IncluderImpl());

SyntaxTree Includer::process(const string &filename, const vector<string> &includeDirs) noexcept {
    SyntaxTree tree(SourceLocation(), "");
    tree.addInclude(filename);
    SyntaxTree new_tree = process(tree, includeDirs);
    return new_tree;
}
