#ifndef SPACEPI_CORE_DTC_LEXER_LEXERIMPL_HPP
#define SPACEPI_CORE_DTC_LEXER_LEXERIMPL_HPP

#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/lexer/Lexer.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>

namespace spacepi {
    namespace dtc {
        namespace lexer {
            /**
             * \brief Implementation of the lexer layer
             */
            class LexerImpl : public Lexer {
                public:
                    /**
                     * \brief Lexes a set of Tokens into Lexemes
                     *
                     * \param[in] tokens The Tokens making up a file
                     * \param[out] lexemes The Lexemes for those Tokens
                     * \return If it was successful
                     */
                    bool lex(const std::vector<tokenizer::Token> &tokens, std::vector<Lexeme> &lexemes) noexcept;
            };
        }
    }
}

#endif
