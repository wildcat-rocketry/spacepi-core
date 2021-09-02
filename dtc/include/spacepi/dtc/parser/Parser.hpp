#ifndef SPACEPI_CORE_DTC_PARSER_PARSER_HPP
#define SPACEPI_CORE_DTC_PARSER_PARSER_HPP

#include <memory>
#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace parser {
            /**
             * \brief Base class for the parser layer logic
             *
             * This layer is responsible for parsing Lexemes into a SyntaxTree.
             */
            class Parser {
                public:
                    /**
                     * \brief Destroys a Parser
                     */
                    virtual ~Parser() noexcept = default;

                    /**
                     * \brief Parses a SyntaxTree out of a file
                     *
                     * \param[in] lexemes The Lexemes from the file
                     * \return The parsed SyntaxTree
                     */
                    virtual SyntaxTree parse(const std::vector<lexer::Lexeme> &lexemes) noexcept = 0;

                    /**
                     * \brief The instance of the Parser layer
                     */
                    static std::shared_ptr<Parser> instance;
            };
        }
    }
}

#endif
