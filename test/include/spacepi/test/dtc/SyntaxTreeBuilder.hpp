#ifndef SPACEPI_TEST_DTC_SYNTAXTREEBUILDER_HPP
#define SPACEPI_TEST_DTC_SYNTAXTREEBUILDER_HPP

#include <spacepi/dtc/parser/SyntaxProperty.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace test {
        namespace dtc {
            /**
             * \brief Model class for a node
             */
            class SyntaxTreeBuilder {
                public:
                    SyntaxTreeBuilder(spacepi::dtc::parser::SyntaxTree tree) noexcept;

                    SyntaxTreeBuilder &addChild(const spacepi::dtc::parser::SyntaxTree &child) noexcept;

                    SyntaxTreeBuilder &addProperty(const spacepi::dtc::parser::SyntaxProperty &prop) noexcept;

                    SyntaxTreeBuilder &addInclude(const std::string &path) noexcept;

                    SyntaxTreeBuilder &setPhandle(uint32_t phandle) noexcept;

                    spacepi::dtc::parser::SyntaxTree build() noexcept;

                private:
                    spacepi::dtc::parser::SyntaxTree tree;
            };
        }
    }
}

#endif
