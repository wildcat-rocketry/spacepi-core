#ifndef SPACEPI_TARGETLIB_LINUX_SHAREDPROCESS_HPP
#define SPACEPI_TARGETLIB_LINUX_SHAREDPROCESS_HPP

#include <initializer_list>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <spacepi/liblinux/UniqueProcess.hpp>

namespace spacepi {
    namespace liblinux {
        class SharedProcess {
            public:
                SharedProcess() = default;

                SharedProcess(bool useInput, bool useOutput, bool useError, const std::string &exe, const std::initializer_list<std::string> &args);
                SharedProcess(bool useInput, bool useOutput, bool useError, const std::string &exe, const std::vector<std::string> &args);

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
