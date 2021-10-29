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
    make_pair(FieldDescriptor::TYPE_DOUBLE, CSharpGen::TypeInfo("double", "Double", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_FLOAT, CSharpGen::TypeInfo("float", "Float", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_INT64, CSharpGen::TypeInfo("long", "Long", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_UINT64, CSharpGen::TypeInfo("ulong", "Ulong", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_INT32, CSharpGen::TypeInfo("int", "Int", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_FIXED64, CSharpGen::TypeInfo("fixed64", "Fixed64", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_FIXED32, CSharpGen::TypeInfo("fixed32", "Fixed32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_BOOL, CSharpGen::TypeInfo("bool", "Bool", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_STRING, CSharpGen::TypeInfo("string", "String", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_GROUP, CSharpGen::TypeInfo("group", "Group", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_MESSAGE, CSharpGen::TypeInfo("message", "Message", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_BYTES, CSharpGen::TypeInfo("byte[]", "Byte[]", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_UINT32, CSharpGen::TypeInfo("uint", "Uint", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_ENUM, CSharpGen::TypeInfo("enum", "Enum", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SFIXED32, CSharpGen::TypeInfo("sfixed32", "Sfixed32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SFIXED64, CSharpGen::TypeInfo("sfixed64", "Sfixed64", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SINT32, CSharpGen::TypeInfo("sint32", "Sint32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SINT64, CSharpGen::TypeInfo("sint64", "Sint64", CSharpGen::DataType::Primitive))
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
    if (property.is_repeated()) {
        os << getFullPropertyData(1, typeMap[property.type()].cSharpType, property.name(), property.number(), property.message_type()) << endl;
    } else {
        os << getFullPropertyData(0, typeMap[property.type()].cSharpType, property.name(), property.number(), property.message_type()) << endl;
    }
}

void CSharpGen::reflectionMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl
       << "// Start reflection()" << endl;
    os << "public " << cls.name() << "() : base(\"" << cls.name() << "\") => Fields = new IField[] {" << indent << endl;
}

void CSharpGen::reflectionMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    if (property.is_repeated()) {
        os << getFullPropertyData(3, typeMap[property.type()].cSharpType, property.name(), property.number(), property.message_type()) << endl;
    } else {
        os << getFullPropertyData(2, typeMap[property.type()].cSharpType, property.name(), property.number(), property.message_type()) << endl;
    }
	os << endl;
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

CSharpGen::TypeInfo::TypeInfo()
    : cSharpType(""), primValue(""), dataType(DataType::Primitive) {
}

string CSharpGen::getFullPropertyData(int structureType, string propertyType, string propertyName, int propertyNumber, const google::protobuf::Descriptor *message_type) const noexcept {
    // I could have made an enum here for this, but I'm just gonna put the convention for this here for now
    // structureType == 0 is just a normal property
    // structureType == 1 is a normal repeatable property
    // structureType == 2 is a reflected Scalar
    // structureType == 3 is a reflected Vector
    if (message_type != NULL) {
        propertyType = message_type->full_name();
    }

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
