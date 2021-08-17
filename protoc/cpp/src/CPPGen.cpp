#include <ostream>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeTemplate.hpp>
#include <spacepi/protoc/CPPGen.hpp>

using namespace std;
using namespace google::protobuf;
using namespace spacepi::protoc;

CPPGen::CPPGen() noexcept : CodeTemplate(".cpp") {
}

void CPPGen::fileBeg(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << "// Start file " << file.name() << endl << indent;
}

void CPPGen::classBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << "// Start class " << cls.name() << endl << indent;
}

void CPPGen::property(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
}

void CPPGen::serializeMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl << "// Start serialize()" << indent << endl;
}

void CPPGen::serializeMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
}

void CPPGen::serializeMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End serialize()" << endl;
}

void CPPGen::parseMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl << "// Start parse()" << indent << endl;
}

void CPPGen::parseMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
}

void CPPGen::parseMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End parse()" << endl;
}

void CPPGen::classEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End class" << endl;
}

void CPPGen::enumBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept {
    os << "// Begin enum " << cls.name() << endl << indent;
}

void CPPGen::enumValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) const noexcept {
    os << "// Enum value " << cls.name() << endl;
}

void CPPGen::enumEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept {
    os << deindent << "// End enum" << endl;
}

void CPPGen::fileEnd(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << deindent << "// End file" << endl;
}
