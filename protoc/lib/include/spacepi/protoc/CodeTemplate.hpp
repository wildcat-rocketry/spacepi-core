#ifndef SPACEPI_CORE_PROTOC_CODETEMPLATE_HPP
#define SPACEPI_CORE_PROTOC_CODETEMPLATE_HPP

#include <string>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeStream.hpp>

namespace spacepi {
    namespace protoc {
        class CodeTemplate {
            public:
                CodeTemplate(const std::string &extension) noexcept;

                std::string outputFilename(const std::string &sourceFile) const noexcept;

                virtual void fileBeg(CodeStream &os, const google::protobuf::FileDescriptor &file) noexcept = 0;
                virtual void classBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) noexcept = 0;
                virtual void property(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) noexcept = 0;
                virtual void reflectionMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) noexcept = 0;
                virtual void reflectionMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) noexcept = 0;
                virtual void reflectionMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) noexcept = 0;
                virtual void classEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) noexcept = 0;
                virtual void enumBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) noexcept = 0;
                virtual void enumValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) noexcept = 0;
                virtual void enumEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) noexcept = 0;
                virtual void enumReflectionBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) noexcept = 0;
                virtual void enumReflectionValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) noexcept = 0;
                virtual void enumReflectionEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) noexcept = 0;
                virtual void fileEnd(CodeStream &os, const google::protobuf::FileDescriptor &file) noexcept = 0;

            private:
                std::string extension;
        };
    }
}

#endif
