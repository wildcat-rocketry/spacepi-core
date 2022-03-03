#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/lexer/LexerImpl.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>

using namespace std;
using namespace spacepi::dtc::lexer;
using namespace spacepi::dtc::tokenizer;

bool LexerImpl::lex(const vector<Token> &tokens, vector<Lexeme> &lexemes) noexcept {
    // TODO
    /* Lexeme locations in vector 'allLexemes'
    0 - VersionTag
    1 - Semicolon
    2 - RootNode
    3 - BraceOpen
    4 - BraceClose
    5 - LabelName
    6 - Colon
    7 - NodeName
    8 - PropertyName
    9 - Equals 
    10 - AngleOpen
    11 - AngleClose
    12 - IntLiteral
    13 - LabelReference
    14 - StringLiteral
    15 - BracketOpen
    16 - BracketClose
    17 - ByteLiteral
    18 - LabelPath
    19 - IncludeTag
     */
    
    /*vector<Lexeme> allLexemes{VersionTag, Semicolon, RootNode, BraceOpen, BraceClose, 
    LabelName, Colon, NodeName, PropertyName, Equals, AngleOpen, AngleClose, IntLiteral,
    LabelReference, StringLiteral, BracketOpen, BracketClose, ByteLiteral, LabelPath, IncludeTag};
    */

    std::string prev, curr, next;

    

    for(int i = 0; i < TOTAL LENGTH; i++)
    {
        if(i == 0){
            curr = token[i].getValue();
            next = token[i+1].getValue();
        }
        else{
            prev = tokens[i-1];
            curr = token[i];
            next = token[i+1];
        }
    }

    if(tokens[1].getValue() == ';')
    {
        lexemes[0] = new Lexeme(Lexeme::Type::VersionTag, token[0].getLocation())
    }
    
    

    return true;
}
