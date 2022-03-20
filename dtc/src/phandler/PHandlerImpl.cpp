#include <algorithm>
#include <stack>
#include <stdint.h>
#include <vector>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/phandler/PHandlerImpl.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::parser;
using namespace spacepi::dtc::phandler;

#define DEBUG_AT(loc) DiagnosticReporter::instance->debug(Diagnostic::PHandler, loc)

void PHandlerImpl::assign(SyntaxTree &tree_) noexcept {
    stack<SyntaxTree *> trees;
    stack<SyntaxTree *> missing;
    uint32_t phandle = 0;
    trees.push(&tree_);
    while (!trees.empty()) {
        SyntaxTree &tree = *trees.top();
        trees.pop();
        for (vector<SyntaxTree>::iterator it = tree.getChildren().begin(); it != tree.getChildren().end(); ++it) {
            trees.push(&*it);
        }
        if (!tree.getPhandle()) {
            missing.push(&tree);
        } else {
            phandle = max(phandle, tree.getPhandle());
        }
    }
    while (!missing.empty()) {
        SyntaxTree &tree = *trees.top();
        trees.pop();
        DEBUG_AT(tree.getLocation()) << "Assigning phandle = " << phandle;
        tree.setPhandle(++phandle);
    }
}
