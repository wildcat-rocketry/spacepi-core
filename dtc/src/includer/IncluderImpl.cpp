#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>
#include <spacepi/dtc/includer/IncludeFinder.hpp>
#include <spacepi/dtc/includer/IncluderImpl.hpp>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/lexer/Lexer.hpp>
#include <spacepi/dtc/parser/Parser.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::includer;
using namespace spacepi::dtc::lexer;
using namespace spacepi::dtc::parser;
using namespace spacepi::dtc::tokenizer;

SyntaxTree IncluderImpl::process(const SyntaxTree &tree, const vector<string> &includeDirs) noexcept {
    SyntaxTree new_node(tree.getLocation(), tree.getName(), tree.getLabel());

    for (const SyntaxProperty &property : tree.getProperties()) {
        new_node.addProperty(property);
    }

    for (const SyntaxTree &child : tree.getChildren()) {
        new_node.addChild(process(child, includeDirs));
    }


    for (vector<string>::const_iterator iit = tree.getIncludes().begin(); iit != tree.getIncludes().end(); ++iit) {

    auto const& path = IncludeFinder::instance->locate(*iit, includeDirs);
        if( path != "" ) {
            vector<Token> tokens;
            Tokenizer::instance->readFile(path, tokens);
            vector<Lexeme> lexemes; 
            Lexer::instance->lex(tokens, lexemes);
            SyntaxTree new_include = Parser::instance->parse(lexemes);
            new_include = process(new_include, includeDirs);
            new_node.mergeTree(new_include);
        } else {
            DiagnosticReporter::instance->report(Diagnostic(Diagnostic::Source::Includer, Diagnostic::Error, "Could not find include file: " + *iit, tree.getLocation()));
        }
    }

    return new_node;
}
