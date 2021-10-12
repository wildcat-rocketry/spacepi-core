#include <ostream>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeTemplate.hpp>
#include <spacepi/protoc/CSharpGen.hpp>

using namespace std;
using namespace google::protobuf;
using namespace spacepi::protoc;

CSharpGen::CSharpGen() noexcept
    : CodeTemplate(".cs") {
}

void CSharpGen::fileBeg(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << "// Start file " << file.name() << endl;
    os << "using System;" << endl;
    os << "using SpacePi.Dashboard.API.Model.Reflection;" << endl;
    os << "using System.Collections.Generic;" << endl;
    os << "namespace " << file.package() << " {" << endl
       << indent;
}

void CSharpGen::classBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << "// Start class " << cls.name() << endl;
    os << "public class " << cls.name() << " : ModelClass, IObject { " << endl
       << indent;
    os << "public IClass Reflection => this;" << endl;
}

void CSharpGen::property(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
    if (property.is_repeated()) {
        switch (property.type()) {
            case FieldDescriptor::TYPE_DOUBLE:
                os << "public List<double> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_FLOAT:
                os << "public List<float> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_INT64:
                os << "public List<long> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_UINT64:
                os << "public List<ulong> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_INT32:
                os << "public List<int> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_FIXED64:
                os << "public List<fixed64> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_FIXED32:
                os << "public List<fixed32> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_BOOL:
                os << "public List<bool> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_STRING:
                os << "public List<string> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_GROUP:
                os << "public List<group> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_MESSAGE:
                os << "public List<" << property.message_type()->full_name() << "> " << property.name() << " { get; set; }" << endl;
                break;
            case FieldDescriptor::TYPE_BYTES:
                os << "public List<byte[]> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_UINT32:
                os << "public List<uint> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_ENUM:
                os << "public List<" << property.enum_type()->full_name() << "> " << property.name() << " { get; set; }" << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED32:
                os << "public List<sfixed32> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED64:
                os << "public List<sfixed64> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_SINT32:
                os << "public List<sint32> " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_SINT64:
                os << "public List<sint64> " << property.name() << " { get; set; } " << endl;
                break;
            default:
                os << "Unknown type - add support!" << endl;
                break;
        }
    } else {
        switch (property.type()) {
            case FieldDescriptor::TYPE_DOUBLE:
                os << "public double " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_FLOAT:
                os << "public float " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_INT64:
                os << "public long " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_UINT64:
                os << "public ulong " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_INT32:
                os << "public int " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_FIXED64:
                os << "public fixed64 " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_FIXED32:
                os << "public fixed32 " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_BOOL:
                os << "public bool " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_STRING:
                os << "public string " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_GROUP:
                os << "public group " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_MESSAGE:
                os << "public " << property.message_type()->full_name() << " " << property.name() << " { get; set; }" << endl;
                break;
            case FieldDescriptor::TYPE_BYTES:
                os << "public byte[] " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_UINT32:
                os << "public uint " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_ENUM:
                os << "public " << property.enum_type()->full_name() << " " << property.name() << " { get; set; }" << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED32:
                os << "public sfixed32 " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED64:
                os << "public sfixed64 " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_SINT32:
                os << "public sint32 " << property.name() << " { get; set; } " << endl;
                break;
            case FieldDescriptor::TYPE_SINT64:
                os << "public sint64 " << property.name() << " { get; set; } " << endl;
                break;
            default:
                os << "Unknown type - debug" << endl;
                break;
        }
    }
}

void CSharpGen::reflectionMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl
       << "// Start reflection()" << endl;
    os << "public " << cls.name() << "() : base(\"" << cls.name() << "\") => Fields = new IField[] {" << indent << endl;
}

void CSharpGen::reflectionMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
    if (property.is_repeated()) {
        switch (property.type()) {
            case FieldDescriptor::TYPE_DOUBLE:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Double, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_FLOAT:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Float, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_INT64:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Int64, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_UINT64:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Uint64, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_INT32:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Int32, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_FIXED64:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, fixed64, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_FIXED32:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, fixed32, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_BOOL:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Bool, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_STRING:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.String, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_GROUP:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, group, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_MESSAGE:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.message_type()->full_name() << ", " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_BYTES:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Bytes, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_UINT32:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Uint32, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_ENUM:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.enum_type()->full_name() << ", " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED32:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sfixed32, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED64:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sfixed64, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_SINT32:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sint32, " << property.name() << ")," << endl;
                break;
            case FieldDescriptor::TYPE_SINT64:
                os << "new VectorPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sint64, " << property.name() << ")," << endl;
                break;
            default:
                os << "Unknown type - debug" << endl;
                break;
        }
    } else {
        switch (property.type()) {
            case FieldDescriptor::TYPE_DOUBLE:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Double, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_FLOAT:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Float, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_INT64:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Int64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_UINT64:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Uint64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_INT32:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Int32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_FIXED64:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, fixed64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_FIXED32:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, fixed32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_BOOL:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Bool, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_STRING:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.String, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_GROUP:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, group, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_MESSAGE:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.message_type()->full_name() << ", () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_BYTES:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Bytes, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_UINT32:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, IPrimitiveField.Types.Uint32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_ENUM:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.enum_type()->full_name() << ", () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED32:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sfixed32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_SFIXED64:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sfixed64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_SINT32:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sint32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            case FieldDescriptor::TYPE_SINT64:
                os << "new ScalarPrimitiveField"
                   << "(\"" << property.name() << "\", " << property.number() << ", false, sint64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
                break;
            default:
                os << "Unknown type - debug" << endl;
                break;
        }
    }
}

void CSharpGen::reflectionMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End reflection()" << endl;
    os << "};" << endl;
}

void CSharpGen::classEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End class" << endl;
    os << "}" << endl;
}

void CSharpGen::enumBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept {
    os << "// Begin enum " << cls.name() << endl
       << indent;
    os << "public enum " << cls.name() << "{" << endl;
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
