#ifndef SPACEPI_CORE_DTC_LEXER_LEXER_HPP
#define SPACEPI_CORE_DTC_LEXER_LEXER_HPP

#include <memory>
#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>

namespace spacepi {
    namespace dtc {
        namespace lexer {
            /**
             * \brief Base class for the lexer layer logic
             *
             * This layer is responsible for categorizing each Token based on its surrounding Tokens
             */
            class Lexer {
                public:
                    /**
                     * \brief Destroys a Lexer
                     */
                    virtual ~Lexer() noexcept = default;

                    /**
                     * \brief Lexes a set of Tokens into Lexemes
                     *
                     * \param[in] tokens The Tokens making up a file
                     * \param[out] lexemes The Lexemes for those Tokens
                     * \return If it was successful
                     */
                    virtual bool lex(const std::vector<tokenizer::Token> &tokens, std::vector<Lexeme> &lexemes) noexcept = 0;

                    /**
                     * \brief The instance of the Lexer layer
                     */
                    static std::shared_ptr<Lexer> instance;
            };
        }
    }
}

#endif
