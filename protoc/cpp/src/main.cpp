#include <google/protobuf/compiler/plugin.h>
#include <spacepi/protoc/CodeGenerator.hpp>
#include <spacepi/protoc/CPPGen.hpp>
#include <spacepi/protoc/HPPGen.hpp>

using namespace google::protobuf;
using namespace spacepi::protoc;

int main(int argc, char **argv) noexcept {
    CPPGen cpp;
    HPPGen hpp;
    CodeGenerator gen { &cpp, &hpp };
    return compiler::PluginMain(argc, argv, &gen);
}
