#include <memory>
#include <spacepi/dtc/lexer/Lexer.hpp>
#include <spacepi/dtc/lexer/LexerImpl.hpp>

using namespace std;
using namespace spacepi::dtc::lexer;

shared_ptr<Lexer> Lexer::instance(new LexerImpl());
