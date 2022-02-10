#ifndef SPACEPI_CORE_TEST_DTC_MOCKPARSER_HPP
#define SPACEPI_CORE_TEST_DTC_MOCKPARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <spacepi/dtc/lexer/Lexeme.hpp>
#include <spacepi/dtc/parser/Parser.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace test {
        namespace dtc {
            /// Mock parser by mapping lexemes to syntax trees
            class MockParser {
                public:
                    MockParser(const std::unordered_map<std::string, spacepi::dtc::parser::SyntaxTree> &tree_mapping) noexcept;
                    ~MockParser() noexcept;

                private:
                    class Impl : public spacepi::dtc::parser::Parser {
                        public:
                            Impl(MockParser *mock) noexcept;

                            // Match the first lexeme with the mapping
                            spacepi::dtc::parser::SyntaxTree parse(const std::vector<spacepi::dtc::lexer::Lexeme> &lexemes) noexcept;
                        private:
                            MockParser *mock;
                    };

                    const std::unordered_map<std::string, spacepi::dtc::parser::SyntaxTree> &tree_mapping;
                    std::shared_ptr<spacepi::dtc::parser::Parser> old;
            };
        }
    }
}

#endif
