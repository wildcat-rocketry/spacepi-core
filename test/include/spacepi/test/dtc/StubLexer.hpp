#ifndef SPACEPI_CORE_TEST_DTC_STUBLEXER_HPP
#define SPACEPI_CORE_TEST_DTC_STUBLEXER_HPP

#include <string>
#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/lexer/Lexer.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>

namespace spacepi {
    namespace test {
        namespace dtc {
            class StubLexer {
                public:
                    StubLexer() noexcept;
                    ~StubLexer() noexcept;
                private:
                    class Impl : public spacepi::dtc::lexer::Lexer {
                        public:
                            // Just converts the tokens to string literalls
                            bool lex(const std::vector<spacepi::dtc::tokenizer::Token> &tokens, std::vector<spacepi::dtc::lexer::Lexeme> &lexemes) noexcept;
                    };

                    std::shared_ptr<spacepi::dtc::lexer::Lexer> old;
            };
        }
    }
}

#endif
