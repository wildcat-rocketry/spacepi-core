#ifndef SPACEPI_CORE_PROTOC_CODEGENERTOR_HPP
#define SPACEPI_CORE_PROTOC_CODEGENERTOR_HPP

#include <initializer_list>
#include <string>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>
#include <spacepi/protoc/CodeTemplate.hpp>

namespace spacepi {
    namespace protoc {
        class CodeGenerator : public google::protobuf::compiler::CodeGenerator {
            public:
                CodeGenerator(const std::initializer_list<const CodeTemplate *> &templates) noexcept;

                bool Generate(const google::protobuf::FileDescriptor *file, const std::string &parameter, google::protobuf::compiler::GeneratorContext *ctx, std::string *error) const noexcept;

            private:
                std::initializer_list<const CodeTemplate *> templates;
        };
    }
}

#endif
