#ifndef SPACEPI_CORE_DTC_PARSER_PARSERIMPL_HPP
#define SPACEPI_CORE_DTC_PARSER_PARSERIMPL_HPP

#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/parser/Parser.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace parser {
            /**
             * \brief Implementation of the parser layer
             */
            class ParserImpl : public Parser {
                public:
                    /**
                     * \brief Parses a SyntaxTree out of a file
                     *
                     * \param[in] lexemes The Lexemes from the file
                     * \return The parsed SyntaxTree
                     */
                    SyntaxTree parse(const std::vector<lexer::Lexeme> &lexemes) noexcept;
            };
        }
    }
}

#endif
