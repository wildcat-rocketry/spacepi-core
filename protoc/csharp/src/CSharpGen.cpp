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
    os << "using System.Collections.Generic;" << endl;
    os << "namespace " << file.package() << " {" << endl << indent;
}

void CSharpGen::classBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << "// Start class " << cls.name() << endl;
    os << "public class " << cls.name() << " : ModelClass { " << endl << indent;
}

void CSharpGen::property(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
    if (property.is_repeated())
    {
        switch (property.type()){
        case 1:
            os << "public List<double> " << property.name() << " { get; set; } " << endl;
            break;
        case 2:
            os << "public List<float> " << property.name() << " { get; set; } " << endl;
            break;
        case 3:
            os << "public List<long> " << property.name() << " { get; set; } " << endl;
            break;
        case 4:
            os << "public List<ulong> " << property.name() << " { get; set; } " << endl;
            break;
        case 5:
            os << "public List<int> " << property.name() << " { get; set; } " << endl;
            break;
        case 6:
            os << "public List<fixed64> " << property.name() << " { get; set; } " << endl;
            break;
        case 7:
            os << "public List<fixed32> " << property.name() << " { get; set; } " << endl;
            break;
        case 8:
            os << "public List<bool> " << property.name() << " { get; set; } " << endl;
            break;
        case 9:
            os << "public List<string> " << property.name() << " { get; set; } " << endl;
            break;
        case 10:
            os << "public List<group> " << property.name() << " { get; set; } " << endl;
            break;
        case 11:
            {
                std::string s = property.message_type()->file()->name();
                s = s.substr(0, s.length() - 6);
                if (property.name() == ("mainWindow")){
                    os << "public google.protobuf.any mainWindow { get; set; } " << endl;
                }
                else if (property.name() == ("Description"))
                {
                    os << "public google.protobuf.descriptor Description { get; set; } " << endl;
                }
                else{
                    os << "public List<" << property.file()->package() << "." << s << "> " << property.name() << " { get; set; } " << endl;
                }
            }
            break;
        case 12:
            os << "public List<bytes> " << property.name() << " { get; set; } " << endl;
            break;
        case 13:
            os << "public List<uint> " << property.name() << " { get; set; } " << endl;
            break;
        case 14:
            os << "public List<enum> " << property.name() << " { get; set; } " << endl;
            break;
        case 15:
            os << "public List<sfixed32> " << property.name() << " { get; set; } " << endl;
            break;
        case 16:
            os << "public List<sfixed64> " << property.name() << " { get; set; } " << endl;
            break;
        case 17:
            os << "public List<sint32> " << property.name() << " { get; set; } " << endl;
            break;
        case 18:
            os << "public List<sint64> " << property.name() << " { get; set; } " << endl;
            break;
        default:
            os << "Unknown type - add support!" << endl;
            break;
        }
    }
    else{
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
                if (property.name() == ("mainWindow")){
                    os << "public google.protobuf.any mainWindow { get; set; } " << endl;
                }
                else if (property.name() == ("Description"))
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
            os << "Unknown type - debug" << endl;
            break;
        }
    }
}

void CSharpGen::reflectionMethodBeg(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << endl << "// Start reflection()" << indent << endl;
    os << "public void Reflection() {" << endl;
}

void CSharpGen::reflectionMethodProperty(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls, const google::protobuf::FieldDescriptor &property) const noexcept {
    os << "// Property " << property.name() << endl;
    if (property.is_repeated()){
        switch (property.type()){
        case 1:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, double, " << property.name() << ")" << endl;
            break;
        case 2:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, float, " << property.name() << ")" << endl;
            break;
        case 3:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, long, " << property.name() << ")" << endl; 
            break;
        case 4:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, ulong, " << property.name() << ")" << endl;
            break;
        case 5:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, int, " << property.name() << ")" << endl;
            break;
        case 6:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, fixed64, " << property.name() << ")" << endl;
            break;
        case 7:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, fixed32, " << property.name() << ")" << endl;
            break;
        case 8:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, bool, " << property.name() << ")" << endl;
            break;
        case 9:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, string, " << property.name() << ")" << endl;
            break;
        case 10:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, group, " << property.name() << ")" << endl;
            break;
        case 11:
            {
                std::string s = property.message_type()->file()->name();
                s = s.substr(0, s.length() - 6);
                if (property.name() == ("mainWindow")){
                    os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, google.protobuf.any, " << property.name() << ")" << endl;
                }
                else if (property.name() == ("Description"))
                {
                    os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, google.protobuf.descriptor, " << property.name() << ")" << endl;
                }
                else{
                    os << "public " << property.file()->package() << "." << s << " " << property.name() << " { get; set; } " << endl;
                }
            }
            break;
        case 12:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, bytes, " << property.name() << ")" << endl;
            break;
        case 13:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, uint, " << property.name() << ")" << endl;
            break;
        case 14:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, enum, " << property.name() << ")" << endl;
            break;
        case 15:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sfixed32, " << property.name() << ")" << endl;
            break;
        case 16:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sfixed64, " << property.name() << ")" << endl;
            break;
        case 17:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sint32, " << property.name() << ")" << endl;
            break;
        case 18:
            os << "public VectorPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sint64, " << property.name() << ")" << endl;
            break;
        default:
            os << "Unknown type - debug" << endl;
            break;
        }
    }
    else{
        switch (property.type()){
        case 1:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, double, x)" << endl;
            break;
        case 2:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, float, x)" << endl;
            break;
        case 3:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, long, x)" << endl;    
            break;
        case 4:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, ulong, x)" << endl;
            break;
        case 5:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, int, x)" << endl;
            break;
        case 6:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, fixed64, x)" << endl;
            break;
        case 7:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, fixed32, x)" << endl;
            break;
        case 8:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, bool, x)" << endl;
            break;
        case 9:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, string, x)" << endl;
            break;
        case 10:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, group, x)" << endl;
            break;
        case 11:
            {
                std::string s = property.message_type()->file()->name();
                s = s.substr(0, s.length() - 6);
                if (property.name() == ("mainWindow")){
                    os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, google.protobuf.any, x)" << endl;
                }
                else if (property.name() == ("Description"))
                {
                    os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, google.protobuf.descriptor, x)" << endl;
                }
                else{
                    os << "public " << property.file()->package() << "." << s << " " << property.name() << " { get; set; } " << endl;
                }
            }
            break;
        case 12:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, bytes, x)" << endl;
            break;
        case 13:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, uint, x)" << endl;
            break;
        case 14:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, enum, x)" << endl;
            break;
        case 15:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sfixed32, x)" << endl;
            break;
        case 16:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sfixed64, x)" << endl;
            break;
        case 17:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sint32, x)" << endl;
            break;
        case 18:
            os << "public ScalarPrimitiveField ref" << property.name() << "( " << property.name() <<", " << property.number() << ", false, sint64, x)" << endl;
            break;
        default:
            os << "Unknown type - debug" << endl;
            break;
        }
    }
}

void CSharpGen::reflectionMethodEnd(CodeStream &os, const google::protobuf::FileDescriptor &file, const google::protobuf::Descriptor &cls) const noexcept {
    os << deindent << "// End reflection()" << endl;
    os << "}" << endl;
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
