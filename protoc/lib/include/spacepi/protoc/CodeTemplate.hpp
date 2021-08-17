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

                virtual void fileBeg                (CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept = 0;
                virtual void classBeg               (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept = 0;
                virtual void property               (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept = 0;
                virtual void serializeMethodBeg     (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept = 0;
                virtual void serializeMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept = 0;
                virtual void serializeMethodEnd     (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept = 0;
                virtual void parseMethodBeg         (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept = 0;
                virtual void parseMethodProperty    (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept = 0;
                virtual void parseMethodEnd         (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept = 0;
                virtual void classEnd               (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept = 0;
                virtual void enumBeg                (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept = 0;
                virtual void enumValue              (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) const noexcept = 0;
                virtual void enumEnd                (CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept = 0;
                virtual void fileEnd                (CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept = 0;

            private:
                std::string extension;
        };
    }
}

#endif
