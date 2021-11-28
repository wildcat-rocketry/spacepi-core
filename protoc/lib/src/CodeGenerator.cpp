#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <queue>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>
#include <spacepi/protoc/CodeGenerator.hpp>
#include <spacepi/protoc/CodeTemplate.hpp>

using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace spacepi::protoc;

CodeGenerator::CodeGenerator(const initializer_list<CodeTemplate *> &templates) noexcept
    : templates(templates) {
}

bool CodeGenerator::Generate(const FileDescriptor *_file, const string &parameter, compiler::GeneratorContext *ctx, string *error) const noexcept {
    const FileDescriptor &file = *_file;
    for (vector<CodeTemplate *>::const_iterator it = templates.begin(); it != templates.end(); ++it) {
        ZeroCopyOutputStream *zos = ctx->Open((*it)->outputFilename(file.name()));
        CodeStream os;
        (*it)->fileBeg(os, file);
        stack<pair<queue<const Descriptor *>, queue<const EnumDescriptor *>>> stack;
        stack.emplace(queue<const Descriptor *>(), queue<const EnumDescriptor *>());
        for (int i = 0; i < file.message_type_count(); ++i) {
            stack.top().first.push(file.message_type(i));
        }
        for (int i = 0; i < file.enum_type_count(); ++i) {
            stack.top().second.push(file.enum_type(i));
        }
        int depth = 0;
        while (!stack.empty()) {
            if (stack.size() > depth) {
                while (!stack.top().second.empty()) {
                    const EnumDescriptor &cls = *stack.top().second.front();
                    (*it)->enumBeg(os, file, cls);
                    for (int j = 0; j < cls.value_count(); ++j) {
                        const EnumValueDescriptor &value = *cls.value(j);
                        (*it)->enumValue(os, file, cls, value);
                    }
                    (*it)->enumEnd(os, file, cls);

                    (*it)->enumReflectionBeg(os, file, cls);
                    for (int j = 0; j < cls.value_count(); ++j) {
                        const EnumValueDescriptor &value = *cls.value(j);
                        (*it)->enumReflectionValue(os, file, cls, value);
                    }
                    (*it)->enumReflectionEnd(os, file, cls);

                    stack.top().second.pop();
                }
                if (stack.top().first.empty()) {
                    stack.pop();
                } else {
                    const Descriptor &cls = *stack.top().first.front();
                    (*it)->classBeg(os, file, cls);
                    stack.emplace(queue<const Descriptor *>(), queue<const EnumDescriptor *>());
                    for (int i = 0; i < cls.nested_type_count(); ++i) {
                        stack.top().first.push(cls.nested_type(i));
                    }
                    for (int i = 0; i < cls.enum_type_count(); ++i) {
                        stack.top().second.push(cls.enum_type(i));
                    }
                    ++depth;
                }
            } else {
                const Descriptor &cls = *stack.top().first.front();
                for (int i = 0; i < cls.field_count(); ++i) {
                    const FieldDescriptor &property = *cls.field(i);
                    (*it)->property(os, file, cls, property);
                }
                (*it)->reflectionMethodBeg(os, file, cls);
                for (int i = 0; i < cls.field_count(); ++i) {
                    const FieldDescriptor &property = *cls.field(i);
                    (*it)->reflectionMethodProperty(os, file, cls, property);
                }
                (*it)->reflectionMethodEnd(os, file, cls);
                (*it)->classEnd(os, file, cls);
                stack.top().first.pop();
                --depth;
                if (stack.top().first.empty()) {
                    stack.pop();
                }
            }
        }
        (*it)->fileEnd(os, file);
        os << flush;
        string str = os.str();
        size_t start = 0;
        while (start < str.size()) {
            void *data;
            int size;
            if (!zos->Next(&data, &size)) {
                break;
            }
            size_t write = size;
            if (write > str.size() - start) {
                write = str.size() - start;
            }
            memcpy(data, str.data() + start, write);
            start += write;
            if (write < size) {
                zos->BackUp((int) (size - write));
            }
        }
    }
    return true;
}
