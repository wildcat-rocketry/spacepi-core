#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <spacepi/dtc/includer/IncluderImpl.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>

using namespace std;
using namespace spacepi::dtc::includer;
using namespace spacepi::dtc::parser;
using namespace spacepi::dtc::tokenizer;

SyntaxTree IncluderImpl::process(const SyntaxTree &tree, const vector<string> &includeDirs) noexcept {
    // TODO
    for (vector<string>::const_iterator iit = tree.getIncludes().begin(); iit != tree.getIncludes().end(); ++iit) {
        for (vector<string>::const_iterator dit = includeDirs.begin(); dit != includeDirs.end(); ++dit) {
            string path = *dit + "/" + *iit;
            ifstream file(path);
            if (file) {
                vector<Token> tokens;
                Tokenizer::instance->readFile(path, tokens);
                for (vector<Token>::const_iterator tit = tokens.begin(); tit != tokens.end(); ++tit) {
                    cout << *tit << endl;
                }
                break;
            }
        }
    }
    return tree;
}
