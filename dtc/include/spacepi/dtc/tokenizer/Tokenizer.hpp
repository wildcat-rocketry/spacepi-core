#ifndef SPACEPI_CORE_DTC_TOKENIZER_TOKENIZER_HPP
#define SPACEPI_CORE_DTC_TOKENIZER_TOKENIZER_HPP

#include <memory>
#include <string>
#include <vector>
#include <spacepi/dtc/tokenizer/Token.hpp>

namespace spacepi {
    namespace dtc {
        namespace tokenizer {
            /**
             * \brief Base class for the tokenizer layer logic
             *
             * This layer is responsible for reading an input file and splitting it into tokens
             */
            class Tokenizer {
                public:
                    /**
                     * \brief Destroys a Tokenizer
                     */
                    virtual ~Tokenizer() noexcept = default;

                    /**
                     * \brief Reads a file and tokenizes it
                     *
                     * \param[in] filename The path to the file to tokenize
                     * \param[out] tokens The tokens contained in the file
                     * \return If it was successful
                     */
                    virtual bool readFile(const std::string &filename, std::vector<Token> &tokens) noexcept = 0;

                    /**
                     * \brief The instance of the Tokenizer layer
                     */
                    static std::shared_ptr<Tokenizer> instance;
            };
        }
    }
}

#endif
