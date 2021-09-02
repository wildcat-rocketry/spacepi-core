#ifndef SPACEPI_CORE_DTC_EMITTER_EMITTERIMPL_HPP
#define SPACEPI_CORE_DTC_EMITTER_EMITTERIMPL_HPP

#include <string>
#include <spacepi/dtc/emitter/Emitter.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace emitter {
            /**
             * \brief Implementation of the emitter layer
             */
            class EmitterImpl : public Emitter {
                public:
                    /**
                     * \brief Writes a device tree to an FDT file.
                     *
                     * \param[in] tree The tree to write
                     * \param[in] filename The path to the file to write
                     * \return If it was successful
                     */
                    bool emit(const parser::SyntaxTree &tree, const std::string &filename) noexcept;
            };
        }
    }
}

#endif
