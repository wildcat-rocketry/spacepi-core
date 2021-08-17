#include <string>
#include <spacepi/protoc/CodeTemplate.hpp>

using namespace std;
using namespace spacepi::protoc;

CodeTemplate::CodeTemplate(const string &extension) noexcept : extension(extension) {
}

string CodeTemplate::outputFilename(const string &sourceFile) const noexcept {
    return sourceFile + extension;
}
