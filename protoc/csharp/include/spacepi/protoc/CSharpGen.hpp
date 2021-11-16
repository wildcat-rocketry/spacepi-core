#ifndef SPACEPI_CORE_PROTOC_CSHARPGEN_HPP
#define SPACEPI_CORE_PROTOC_CSHARPGEN_HPP

#include <string>
#include <unordered_map>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeTemplate.hpp>

namespace spacepi {
    namespace protoc {
        class CSharpGen : public CodeTemplate {
            public:
                CSharpGen() noexcept;

                void fileBeg(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept;
                void classBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept;
                void property(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept;
                void reflectionMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept;
                void reflectionMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept;
                void reflectionMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept;
                void enumReflectionBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept;
                void enumReflectionValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) const noexcept;
                void enumReflectionEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept;
                void classEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept;
                void enumBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept;
                void enumValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) const noexcept;
                void enumEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept;
                void fileEnd(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept;

            private:
                enum class DataType
                {
                    Class,
                    Enum,
                    Primitive
                };

                enum class StructureType
                {
                    Scalar,
                    Vector,
                    ScalarReflection,
                    VectorReflection
                };

                class TypeInfo {
                    public:
                        std::string cSharpType;
                        std::string primValue;
                        DataType dataType;

                        TypeInfo(const std::string &cSharpType, const std::string &primValue, DataType dataType);
                        TypeInfo(); // makepair needs this constructor
                };

                static std::unordered_map<google::protobuf::FieldDescriptor::Type, TypeInfo> typeMap;

                void getFullPropertyData(CodeStream &os, StructureType type, const google::protobuf::FieldDescriptor &property) const noexcept;
                std::string getPropertyType(int propertyType) const noexcept;
                std::string overrideKeyword(std::string propertyName) const noexcept;
        };
    }
}

#endif
