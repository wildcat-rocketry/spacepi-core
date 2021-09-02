#ifndef SPACEPI_CORE_DTC_EMITTER_EMITTER_HPP
#define SPACEPI_CORE_DTC_EMITTER_EMITTER_HPP

#include <memory>
#include <string>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace emitter {
            /**
             * \brief Base class for the emitter layer logic
             *
             * This layer is responsible for writing the device tree to an FDT file.
             */
            class Emitter {
                public:
                    /**
                     * \brief Destroys an Emitter
                     */
                    virtual ~Emitter() noexcept = default;

                    /**
                     * \brief Writes a device tree to an FDT file.
                     *
                     * \param[in] tree The tree to write
                     * \param[in] filename The path to the file to write
                     * \return If it was successful
                     */
                    virtual bool emit(const parser::SyntaxTree &tree, const std::string &filename) noexcept = 0;

                    /**
                     * \brief The instance of the Emitter layer
                     */
                    static std::shared_ptr<Emitter> instance;
            };
        }
    }
}

#endif
