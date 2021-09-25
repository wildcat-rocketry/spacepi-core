#include <memory>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>
#include <spacepi/dtc/tokenizer/TokenizerImpl.hpp>

using namespace std;
using namespace spacepi::dtc::tokenizer;

shared_ptr<Tokenizer> Tokenizer::instance(new TokenizerImpl());
