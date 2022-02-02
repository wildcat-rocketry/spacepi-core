#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <spacepi/dtc/includer/IncluderImpl.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/lexer/Lexer.hpp>
#include <spacepi/dtc/parser/Parser.hpp>

using namespace std;
using namespace spacepi::dtc::includer;
using namespace spacepi::dtc::parser;
using namespace spacepi::dtc::lexer;
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
        bool found = false;
        for (vector<string>::const_iterator dit = includeDirs.begin(); dit != includeDirs.end(); ++dit) {
            string path = *dit + "/" + *iit;
            ifstream file(path);
            if (file) {
                vector<Token> tokens;
                Tokenizer::instance->readFile(path, tokens);
                vector<Lexeme> lexemes; 
                Lexer::instance->lex(tokens, lexemes);
                SyntaxTree new_include = Parser::instance->parse(lexemes);
                process(new_include, includeDirs);
                new_node.mergeTree(new_include);

                found = true;

                break;
            }
        }

        if(!found) {
            cerr << "Could not find include file: " << *iit << endl;
        }
    }

    return new_node;
}
