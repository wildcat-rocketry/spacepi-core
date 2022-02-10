#ifndef SPACEPI_CORE_TEST_DTC_STUBTOKENIZER_HPP
#define SPACEPI_CORE_TEST_DTC_STUBTOKENIZER_HPP

#include <string>
#include <vector>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>

namespace spacepi {
    namespace test {
        namespace dtc {
            class StubTokenizer {
                public:
                    StubTokenizer() noexcept;
                    ~StubTokenizer() noexcept;
                private:
                    class Impl : public spacepi::dtc::tokenizer::Tokenizer {
                        public:
                            /// Just creates one token with filename
                            bool readFile(const std::string &filename, std::vector<spacepi::dtc::tokenizer::Token> &tokens) noexcept override;
                    };

                    std::shared_ptr<spacepi::dtc::tokenizer::Tokenizer> old;
            };
        }
    }
}

#endif
