#ifndef SPACEPI_CORE_DTC_TOKENIZER_TOKENIZERIMPL_HPP
#define SPACEPI_CORE_DTC_TOKENIZER_TOKENIZERIMPL_HPP

#include <string>
#include <vector>
#include <spacepi/dtc/tokenizer/Token.hpp>
#include <spacepi/dtc/tokenizer/Tokenizer.hpp>

namespace spacepi {
    namespace dtc {
        namespace tokenizer {
            /**
             * \brief Implementation of the tokenizer layer
             */
            class TokenizerImpl : public Tokenizer {
                public:
                    /**
                     * \brief Reads a file and tokenizes it
                     *
                     * \param[in] filename The path to the file to tokenize
                     * \param[out] tokens The tokens contained in the file
                     * \return If it was successful
                     */
                    bool readFile(const std::string &filename, std::vector<Token> &tokens) noexcept;
            };
        }
    }
}

#endif
