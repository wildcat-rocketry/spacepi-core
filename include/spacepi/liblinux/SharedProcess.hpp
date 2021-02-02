#ifndef SPACEPI_TARGETLIB_LINUX_SHAREDPROCESS_HPP
#define SPACEPI_TARGETLIB_LINUX_SHAREDPROCESS_HPP

#include <istream>
#include <memory>
#include <ostream>
#include <spacepi/liblinux/UniqueProcess.hpp>

namespace spacepi {
    namespace liblinux {
        class SharedProcess {
            public:
                SharedProcess() = default;

                template <typename... Args>
                SharedProcess(bool useInput, bool useOutput, bool useError, const std::string &exe, const Args &... args) : unique(new UniqueProcess(useInput, useOutput, useError, exe, args...)) {
                }

                std::ostream &input() noexcept;
                std::istream &output() noexcept;
                std::istream &error() noexcept;

                void closeInput();

                bool running();
                void wait();
                int getExitCode() const;

            private:
                std::shared_ptr<UniqueProcess> unique;
        };
    }
}

#endif
