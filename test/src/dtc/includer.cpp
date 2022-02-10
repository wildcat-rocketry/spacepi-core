#include <unordered_map>
#include <vector>
#include <gtest/gtest.h>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/includer/Includer.hpp>
#include <spacepi/dtc/parser/SyntaxProperty.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/test/dtc/MockDiagnosticReporter.hpp>
#include <spacepi/test/dtc/MockIncludeFinder.hpp>
#include <spacepi/test/dtc/MockParser.hpp>
#include <spacepi/test/dtc/StubLexer.hpp>
#include <spacepi/test/dtc/StubTokenizer.hpp>
#include <spacepi/test/dtc/SyntaxTreeBuilder.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::includer;
using namespace spacepi::dtc::parser;
using namespace spacepi::test::dtc;

TEST(dtc_includer, singleFile) {
    StubTokenizer tokenizer;
    StubLexer lexer;
    MockIncludeFinder finder;
    unordered_map<string, SyntaxTree> tree_mapping({
        {
            "foo.dtc",
            SyntaxTreeBuilder(SyntaxTree(SourceLocation(), "", ""))
                .setPhandle(3)
                .addProperty(SyntaxProperty(SourceLocation(), "foo", "bar"))
                .build()
        },
    });

    MockParser parser(tree_mapping);
    EXPECT_EQ(tree_mapping.at("foo.dtc"), Includer::instance->process("foo.dtc", {}));
}

TEST(dtc_includer, includeOneFile) {
    StubTokenizer tokenizer;
    StubLexer lexer;
    MockIncludeFinder finder;
    unordered_map<string, SyntaxTree> tree_mapping({
        {
            "foo.dtc",
            SyntaxTreeBuilder(SyntaxTree(SourceLocation(), "", ""))
                .setPhandle(3)
                .addProperty(SyntaxProperty(SourceLocation(), "foo", "bar"))
                .addInclude("bar.dtc")
                .build()
        },
        {
            "bar.dtc",
            SyntaxTreeBuilder(SyntaxTree(SourceLocation(), "", ""))
                .addProperty(SyntaxProperty(SourceLocation(), "yeet", "yote"))
                .build()
        },
        {
            "yoted.dtc",
            SyntaxTreeBuilder(SyntaxTree(SourceLocation(), "", ""))
                .setPhandle(3)
                .addProperty(SyntaxProperty(SourceLocation(), "foo", "bar"))
                .addProperty(SyntaxProperty(SourceLocation(), "yeet", "yote"))
                .build()
        },
    });

    MockParser parser(tree_mapping);
    EXPECT_EQ(tree_mapping.at("yoted.dtc"), Includer::instance->process("foo.dtc", {}));
}
