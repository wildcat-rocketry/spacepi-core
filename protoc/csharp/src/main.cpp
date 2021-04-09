#include <google/protobuf/compiler/plugin.h>
#include <spacepi/protoc/CodeGenerator.hpp>
#include <spacepi/protoc/CSharpGen.hpp>

using namespace google::protobuf;
using namespace spacepi::protoc;

int main(int argc, char **argv) noexcept {
    CSharpGen cs;
    CodeGenerator gen { &cs };
    return compiler::PluginMain(argc, argv, &gen);
}
