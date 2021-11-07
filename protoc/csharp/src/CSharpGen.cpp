#include <ostream>
#include <string>
#include <utility>
#include <google/protobuf/descriptor.pb.h>
#include <spacepi/protoc/CodeTemplate.hpp>
#include <spacepi/protoc/CSharpGen.hpp>

#include <spacepi/protoc/transient.pb.h>

using namespace std;
using namespace google::protobuf;
using namespace spacepi::protoc;

unordered_map<FieldDescriptor::Type, CSharpGen::TypeInfo> CSharpGen::typeMap = {
    make_pair(FieldDescriptor::TYPE_DOUBLE, CSharpGen::TypeInfo("double", "Double", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_FLOAT, CSharpGen::TypeInfo("float", "Float", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_INT64, CSharpGen::TypeInfo("long", "Int64", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_UINT64, CSharpGen::TypeInfo("ulong", "Uint64", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_INT32, CSharpGen::TypeInfo("int", "Int32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_FIXED64, CSharpGen::TypeInfo("fixed64", "Uint64", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_FIXED32, CSharpGen::TypeInfo("fixed32", "Uint32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_BOOL, CSharpGen::TypeInfo("bool", "Bool", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_STRING, CSharpGen::TypeInfo("string", "String", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_GROUP, CSharpGen::TypeInfo("group", "", CSharpGen::DataType::Class)),
    make_pair(FieldDescriptor::TYPE_MESSAGE, CSharpGen::TypeInfo("message", "", CSharpGen::DataType::Class)),
    make_pair(FieldDescriptor::TYPE_BYTES, CSharpGen::TypeInfo("byte[]", "Bytes", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_UINT32, CSharpGen::TypeInfo("uint", "Uint32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_ENUM, CSharpGen::TypeInfo("enum", "", CSharpGen::DataType::Enum)),
    make_pair(FieldDescriptor::TYPE_SFIXED32, CSharpGen::TypeInfo("sfixed32", "Int32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SFIXED64, CSharpGen::TypeInfo("sfixed64", "Int64", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SINT32, CSharpGen::TypeInfo("sint32", "Int32", CSharpGen::DataType::Primitive)),
    make_pair(FieldDescriptor::TYPE_SINT64, CSharpGen::TypeInfo("sint64", "Int64", CSharpGen::DataType::Primitive))
};

CSharpGen::CSharpGen() noexcept
    : CodeTemplate(".cs") {
}

void CSharpGen::fileBeg(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << "// Start file " << file.name() << endl;
    os << "using System;" << endl;
    os << "using SpacePi.Dashboard.API.Model.Reflection;" << endl;
    os << "using System.Collections.Generic;" << endl;
    os << "using System.Collections.ObjectModel;" << endl;
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
        getFullPropertyData(os, 1, property);
    } else {
        getFullPropertyData(os, 0, property);
    }

    for (int i = 0; i < property.options().uninterpreted_option_size(); i++) {
        UninterpretedOption uo = property.options().uninterpreted_option(i);
        os << "/*  UninterpretedOption: ";
        for (int j = 0; j < uo.name_size(); i++) {
            os << uo.name(j).name_part() << ".";
        }
        os << "  */" << endl;
    }
}

void CSharpGen::reflectionMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl
       << "// Start reflection()" << endl;
    os << "public " << cls.name() << "() : base(\"" << cls.name() << "\") => base.Fields = new IField[] {" << indent << endl;
}

void CSharpGen::reflectionMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    if (property.is_repeated()) {
        getFullPropertyData(os, 3, property);
    } else {
        getFullPropertyData(os, 2, property);
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
    os << "public enum " << cls.name() << "{" << endl << indent;
}

void CSharpGen::enumValue(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls, const google::protobuf::EnumValueDescriptor &value) const noexcept {
    os << value.name() << " = " << value.number() << ", " << endl;
}

void CSharpGen::enumEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::EnumDescriptor &cls) const noexcept {
    os << deindent << "}" << endl;
}

void CSharpGen::fileEnd(CodeStream &os, const google::protobuf::FileDescriptor &file) const noexcept {
    os << "// End file" << endl;
    os << deindent << "}" << endl;
}

void CSharpGen::enumReflectionBeg(CodeStream& os, const google::protobuf::FileDescriptor& file, const google::protobuf::EnumDescriptor& cls) const noexcept {
    os << "public class " << cls.name() << "__Reflection : ModelEnum {" << endl;
    os << indent;
    os << "public " << cls.name() << "__Reflection() : base(\"" << cls.name() << "\"" << endl << indent;
}

void CSharpGen::enumReflectionValue(CodeStream& os, const google::protobuf::FileDescriptor& file, const google::protobuf::EnumDescriptor& cls, const google::protobuf::EnumValueDescriptor& value) const noexcept {
    os << ", (" << value.number() << ",\"" << value.name() << "\")" << endl;
}

void CSharpGen::enumReflectionEnd(CodeStream& os, const google::protobuf::FileDescriptor& file, const google::protobuf::EnumDescriptor& cls) const noexcept {
    os << deindent;
    os << ") { }" << endl;
    os << deindent << "}" << endl;
}

CSharpGen::TypeInfo::TypeInfo(const string &cSharpType, const string &primValue, DataType dataType)
    : cSharpType(cSharpType), primValue(primValue), dataType(dataType) {
}

CSharpGen::TypeInfo::TypeInfo()
    : cSharpType(""), primValue(""), dataType(DataType::Primitive) {
}

void CSharpGen::getFullPropertyData(CodeStream &os, int structureType, const google::protobuf::FieldDescriptor &property) const noexcept {
    // I could have made an enum here for this, but I'm just gonna put the convention for this here for now
    // structureType == 0 is just a normal property
    // structureType == 1 is a normal repeatable property
    // structureType == 2 is a reflected Scalar
    // structureType == 3 is a reflected Vector
    string propertyType = "";
    string primType = "";
    string propertyName = property.name();
	string propertyNumber = to_string(property.number());

    switch (property.type()) {
		case FieldDescriptor::TYPE_MESSAGE:
		case FieldDescriptor::TYPE_GROUP:
            propertyType = property.message_type()->full_name();
            break;
        case FieldDescriptor::TYPE_ENUM:
			propertyType = property.enum_type()->full_name();
            break;
        default:
            propertyType = typeMap[property.type()].cSharpType;
            primType = typeMap[property.type()].primValue;
            break;
    }

    string trans = (property.options().GetExtension(transient))? "true": "false";

    switch (structureType) {
        case 0:
            os << "public " + propertyType + " " + propertyName + " { get; set; } " << endl;
            if (property.type() == FieldDescriptor::TYPE_ENUM) {
                os << "private readonly " << propertyType << "__Reflection " << propertyName << "__ReflectionObject = new();" << endl;
            }
            break;
        case 1:
            os << "public ObservableCollection<" + propertyType + "> " + propertyName + " { get; private set; } = new();" << endl;
            if (property.type() == FieldDescriptor::TYPE_ENUM) {
                os << "private readonly " << propertyType << "__Reflection " << propertyName << "__ReflectionObject = new();" << endl;
            }
            break;
        case 2:
            switch (property.type()) {
				case FieldDescriptor::TYPE_MESSAGE:
                case FieldDescriptor::TYPE_GROUP:
                    os << "new ScalarClassField<" + propertyType + ">(\"" + propertyName + "\", " + propertyNumber + ", " + trans + ", () => { return " + propertyName + "; }, (v) => { " + propertyName + " = (" + propertyType + ")v; }), " << endl;
                    break;
                case FieldDescriptor::TYPE_ENUM:
                    os << "new ScalarEnumField(\"" + propertyName + "\", " + propertyNumber + ", " + trans + ", " + propertyName + "__ReflectionObject, () => { return (int)" + propertyName + "; }, (v) => { " + propertyName + " = (" + propertyType + ")v; }), " << endl;
                    break;
                default:
					os << "new ScalarPrimitiveField(\"" + propertyName + "\", " + propertyNumber + ", " + trans + ", IPrimitiveField.Types." + primType + ", () => { return " + propertyName + "; }, (v) => { " + propertyName + " = (" + propertyType + ")v; }), " << endl;
                    break;
            }
            break;
        case 3:
            switch (property.type()) {
				case FieldDescriptor::TYPE_MESSAGE:
				case FieldDescriptor::TYPE_GROUP:
                    os << "new VectorClassField<" + propertyType + ">(\"" + propertyName + "\", " + propertyNumber + ", " + trans + ", " + propertyName + ", () => { return new " + propertyType + "(); }), " << endl;
                    break;
                case FieldDescriptor::TYPE_ENUM:
                    os << "new VectorEnumField<" + propertyType + ">(\"" + propertyName + "\", " + propertyNumber + ", " + trans + ", " + propertyName + ", (e) => { return (int)e; }, (i) => { return (" + propertyType + ")i; }, " + propertyName + "__ReflectionObject ), " << endl;
                    break;
                default:
					os << "new VectorPrimitiveField<" + propertyType + ">(\"" + propertyName + "\", " + propertyNumber + ", " + trans + ", IPrimitiveField.Types." + primType + ", " + propertyName + "), " << endl;
                    break;
            }
            break;
        default:
            os << "#error Protobuf compiler encountered unknown structureType " << structureType << endl;
    }
}
