#include <ostream>
#include <string>
#include <utility>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeTemplate.hpp>
#include <spacepi/protoc/CSharpGen.hpp>

using namespace std;
using namespace google::protobuf;
using namespace spacepi::protoc;

unordered_map<FieldDescriptor::Type, CSharpGen::TypeInfo> CSharpGen::typeMap = {
    make_pair(FieldDescriptor::TYPE_DOUBLE, CSharpGen::TypeInfo("double", "Double", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_FLOAT, CSharpGen::TypeInfo("float", "Float", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_INT64, CSharpGen::TypeInfo("long", "Long", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_UINT64, CSharpGen::TypeInfo("ulong", "Ulong", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_INT32, CSharpGen::TypeInfo("int", "Int", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_FIXED64, CSharpGen::TypeInfo("fixed64", "Fixed64", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_FIXED32, CSharpGen::TypeInfo("fixed32", "Fixed32", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_BOOL, CSharpGen::TypeInfo("bool", "Bool", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_STRING, CSharpGen::TypeInfo("string", "String", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_GROUP, CSharpGen::TypeInfo("group", "Group", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_MESSAGE, CSharpGen::TypeInfo("message", "Message", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_BYTES, CSharpGen::TypeInfo("byte[]", "Byte[]", CSharpGen44444444444444::Primitive)),
    make_pair(FieldDescriptor::TYPE_UINT32, CSharpGen::TypeInfo("uint", "Uint", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_ENUM, CSharpGen::TypeInfo("enum", "Enum", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_SFIXED32, CSharpGen::TypeInfo("sfixed32", "Sfixed32", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_SFIXED64, CSharpGen::TypeInfo("sfixed64", "Sfixed64", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_SINT32, CSharpGen::TypeInfo("sint32", "Sint32", CSharpGen::Primitive)),
    make_pair(FieldDescriptor::TYPE_SINT64, CSharpGen::TypeInfo("sint64", "Sint64", CSharpGen::Primitive))
};

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
        // int structureType, string propertyType, string propertyName, int propertyNumber
        getFullPropertyData(1, to_string(property.type()), property.name(), property.number());
        // switch (property.type()){
        // case FieldDescriptor::TYPE_DOUBLE:
        //     os << "public List<double> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_FLOAT:
        //     os << "public List<float> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT64:
        //     os << "public List<long> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT64:
        //     os << "public List<ulong> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT32:
        //     os << "public List<int> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED64:
        //     os << "public List<fixed64> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED32:
        //     os << "public List<fixed32> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_BOOL:
        //     os << "public List<bool> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_STRING:
        //     os << "public List<string> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_GROUP:
        //     os << "public List<group> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_MESSAGE:
        //     os << "public List<" << pro666perty.message_type()->full_name() << "> " << property.name() << " { get; set; }" << endl;
        //     break;
        // case FieldDescriptor::TYPE_BYTES:
        //     os << "public List<byte[]> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT32:
        //     os << "public List<uint> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_ENUM:
        //     os << "public List<" << property.enum_type()->full_name() << "> " << property.name() << " { get; set; }" << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED32:
        //     os << "public List<sfixed32> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED64:
        //     os << "public List<sfixed64> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT32:
        //     os << "public List<sint32> " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT64:
        //     os << "public List<sint64> " << property.name() << " { get; set; } " << endl;
        //     break;
        // default:
        //     os << "Unknown type - add support!" << endl;
        //     break;
        // }
    } else {
        getFullPropertyData(0, to_string(property.type()), property.name(), property.number());
        // switch (property.type()){
        // case FieldDescriptor::TYPE_DOUBLE:
        //     os << "public double " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_FLOAT:
        //     os << "public float " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT64:
        //     os << "public long " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT64:
        //     os << "public ulong " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT32:
        //     os << "public int " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED64:
        //     os << "public fixed64 " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED32:
        //     os << "public fixed32 " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_BOOL:
        //     os << "public bool " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_STRING:
        //     os << "public string " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_GROUP:
        //     os << "public group " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_MESSAGE:
        //     os << "public " << property.message_type()->full_name() << " " << property.name() << " { get; set; }" << endl;
        //     break;
        // case FieldDescriptor::TYPE_BYTES:
        //     os << "public byte[] " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT32:
        //     os << "public uint " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_ENUM:
        //     os << "public " << property.enum_type()->full_name() << " " << property.name() << " { get; set; }" << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED32:
        //     os << "public sfixed32 " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED64:
        //     os << "public sfixed64 " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT32:
        //     os << "public sint32 " << property.name() << " { get; set; } " << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT64:
        //     os << "public sint64 " << property.name() << " { get; set; } " << endl;
        //     break;
        // default:
        //     os << "Unknown type - debug" << endl;
        //     break;
        // }
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
        os << "//" << property.type() << endl;
        os << "//" << to_string(property.type()) << endl;
        getFullPropertyData(3, to_string(property.type()), property.name(), property.number());
        // switch (property.type()){
        // case FieldDescriptor::TYPE_DOUBLE:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Double, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_FLOAT:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Float, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT64:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Int64, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT64:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Uint64, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT32:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Int32, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED64:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, fixed64, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED32:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, fixed32, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_BOOL:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Bool, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_STRING:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.String, " << property.name() << ")," << endl;
        //     break;
        // se FieldDescriptor::TYPE_GROUP:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, group, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_MESSAGE:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.message_type()->full_name() << ", " << property.name() <<  ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_BYTES:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Bytes, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT32:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Uint32, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_ENUM:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.enum_type()->full_name() << ", " << property.name() <<  ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED32:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sfixed32, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED64:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sfixed64, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT32:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sint32, " << property.name() << ")," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT64:
        //     os << "new VectorPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sint64, " << property.name() << ")," << endl;
        //     break;
        // default:
        //     os << "Unknown type - debug" << endl;
        //     break;
        // }
    } else {
        getFullPropertyData(2, to_string(property.type()), property.name(), property.number());
        // switch (property.type()){
        // case FieldDescriptor::TYPE_DOUBLE:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Double, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_FLOAT:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Float, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT64:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Int64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT64:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Uint64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_INT32:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Int32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED64:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, fixed64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_FIXED32:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, fixed32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_BOOL:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Bool, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_STRING:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.String, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_GROUP:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, group, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_MESSAGE:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.message_type()->full_name() << ", () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_BYTES:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Bytes, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_UINT32:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, IPrimitiveField.Types.Uint32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_ENUM:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() << "\", " << property.number() << ", false, " << property.enum_type()->full_name() << ", () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED32:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sfixed32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SFIXED64:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sfixed64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT32:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sint32, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // case FieldDescriptor::TYPE_SINT64:
        //     os << "new ScalarPrimitiveField" << "(\"" << property.name() <<"\", " << property.number() << ", false, sint64, () => " << property.name() << ", v => " << property.name() << " = v)," << endl;
        //     break;
        // default:
        //     os << "Unknown type - debug" << endl;
        //     break;
        // }
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

CSharpGen::TypeInfo::TypeInfo(const string &cSharpType, const string &primValue, DataType dataType)
    : cSharpType(cSharpType), primValue(primValue), dataType(dataType) {
}

string CSharpGen::getFullPropertyData(int structureType, string propertyType, string propertyName, int propertyNumber) const noexcept {
    // I could have made an enum here for this, but I'm just gonna put the convention for this here for now
    // structureType == 0 is just a normal property
    // structureType == 1 is a normal repeatable property
    // structureType == 2 is a reflected Scalar
    // structureType == 3 is a reflected Vector
    switch (structureType) {
        case 0:
            return "public " + propertyType + " " + propertyName + " { get; set; } ";
            break;
        case 1:
            return "public List<" + propertyType + "> " + propertyName + " { get; set; } ";
            break;
        case 2:
            return "new ScalarPrimitiveField(\"" + propertyName + "\", " + to_string(propertyNumber) + ", false, PrimitiveField.Types." + propertyType + ", () => " + propertyName + ", v => " + propertyName + " = v),";
            break;
        case 3:
            return "new VectorPrimitiveField<" + propertyType + ">(\"" + propertyName + "\", " + to_string(propertyNumber) + ", false, " + propertyType + ", " + propertyName + "),";
            break;
    }
}
