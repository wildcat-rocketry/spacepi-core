#include <ostream>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeTemplate.hpp>
#include <spacepi/protoc/CSharpGen.hpp>

using namespace std;
using namespace google::protobuf;
using namespace spacepi::protoc;

CSharpGen::CSharpGen() noexcept : CodeTemplate(".cs") {
}

void CSharpGen::fileBeg(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << "// Start file " << file.name() << endl;
    os << "using System;" << endl;
    os << "using SpacePi.Dashboard.API.Model.Reflection;" << endl;
    os << "namespace " << file.package() << " {" << endl << indent;
}

void CSharpGen::classBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << "// Start class " << cls.name() << endl;
    os << "public class " << cls.name() << " : ModelClass { " << endl << indent;
}

void CSharpGen::property(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
    switch (property.type()){
        case 1:
            os << "public double " << property.name() << " { get; set; } " << endl;
            break;
        case 2:
            os << "public float " << property.name() << " { get; set; } " << endl;
            break;
        case 3:
            os << "public long " << property.name() << " { get; set; } " << endl;
            break;
        case 4:
            os << "public ulong " << property.name() << " { get; set; } " << endl;
            break;
        case 5:
            os << "public int " << property.name() << " { get; set; } " << endl;
            break;
        case 6:
            os << "public fixed64 " << property.name() << " { get; set; } " << endl;
            break;
        case 7:
            os << "public fixed32 " << property.name() << " { get; set; } " << endl;
            break;
        case 8:
            os << "public bool " << property.name() << " { get; set; } " << endl;
            break;
        case 9:
            os << "public string " << property.name() << " { get; set; } " << endl;
            break;
        case 10:
            os << "public group " << property.name() << " { get; set; } " << endl;
            break;
        case 11:
            {
                std::string s = property.message_type()->file()->name();
                s = s.substr(0, s.length() - 6);
                if (property.name()._Equal("mainWindow")){
                    os << "public google.protobuf.any mainWindow { get; set; } " << endl;
                }
                else if (property.name()._Equal("Description"))
                {
                    os << "public google.protobuf.descriptor Description { get; set; } " << endl;
                }
                else{
                    os << "public " << property.file()->package() << "." << s << " " << property.name() << " { get; set; } " << endl;
                }
            }
            break;
        case 12:
            os << "public bytes " << property.name() << " { get; set; } " << endl;
            break;
        case 13:
            os << "public uint " << property.name() << " { get; set; } " << endl;
            break;
        case 14:
            os << "public enum " << property.name() << " { get; set; } " << endl;
            break;
        case 15:
            os << "public sfixed32 " << property.name() << " { get; set; } " << endl;
            break;
        case 16:
            os << "public sfixed64 " << property.name() << " { get; set; } " << endl;
            break;
        case 17:
            os << "public sint32 " << property.name() << " { get; set; } " << endl;
            break;
        case 18:
            os << "public sint64 " << property.name() << " { get; set; } " << endl;
            break;
        default:
            os << "f" << endl;
            break;
    }
}

void CSharpGen::serializeMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl << "// Start serialize()" << indent << endl;
}

void CSharpGen::serializeMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
}

void CSharpGen::serializeMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End serialize()" << endl;
}

void CSharpGen::parseMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl << "// Start parse()" << indent << endl;
}

void CSharpGen::parseMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
}

void CSharpGen::parseMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End parse()" << endl;
}

void CSharpGen::classEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End class" << endl;
    os << "}" << endl;
}

void CSharpGen::enumBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept {
    os << "// Begin enum " << cls.name() << endl << indent;
    os << "public enum " << cls.name() << endl;
}

void CSharpGen::enumValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) const noexcept {
    os << "// Enum value " << cls.name() << endl;
    os << value.name() << " ," << endl;
}

void CSharpGen::enumEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept {
    os << "// End enum" << endl;
    os << deindent << "}" << endl;
}

void CSharpGen::fileEnd(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << "// End file" << endl;
    os << deindent << "}" << endl;
}
