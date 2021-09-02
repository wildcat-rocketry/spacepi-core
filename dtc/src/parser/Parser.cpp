#include <memory>
#include <spacepi/dtc/parser/Parser.hpp>
#include <spacepi/dtc/parser/ParserImpl.hpp>

using namespace std;
using namespace spacepi::dtc::parser;

shared_ptr<Parser> Parser::instance(new ParserImpl());
