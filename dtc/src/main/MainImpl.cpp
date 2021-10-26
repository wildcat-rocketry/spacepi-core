#include <exception>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <gflags/gflags.h>
#include <spacepi/dtc/emitter/Emitter.hpp>
#include <spacepi/dtc/includer/Includer.hpp>
#include <spacepi/dtc/main/Main.hpp>
#include <spacepi/dtc/main/MainImpl.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/validator/Validator.hpp>

using namespace std;
using namespace google;
using namespace spacepi::dtc::emitter;
using namespace spacepi::dtc::includer;
using namespace spacepi::dtc::main;
using namespace spacepi::dtc::parser;
using namespace spacepi::dtc::validator;

DEFINE_string(source_file, "", "The .dts source file to compile");
DEFINE_string(output_file, "", "The .dtb file to generate");
DEFINE_string(schema_file, "", "The .dts schema file to check the source_file against");
DEFINE_string(include_dirs, "", "A semicolon-separated list of directories to search for include files in");

int MainImpl::run(int argc, const char **argv) noexcept {
    try {
        vector<char *> argvm;
        vector<vector<char>> argvmb;
        argvm.reserve(argc);
        argvmb.reserve(argc);
        for (int i = 0; i < argc; ++i) {
            argvmb.emplace_back(argv[i], argv[i] + strlen(argv[i]) + 1);
            argvm.push_back(argvmb.back().data());
        }
        char **argvmp = argvm.data();
        ParseCommandLineFlags(&argc, &argvmp, true);
        switch (argc) {
            case 0:
#define STRING(x) #x
#define THIS_LINE STRING(__LINE__)
                cerr << "Internal error in " __FILE__ ":" THIS_LINE << endl;
#undef THIS_LINE
#undef STRING
                return EXIT_FAILURE;
            case 1:
                break;
            default:
                cerr << "ERROR: unknown command line flag '" << argvmp[1] << "'" << endl;
                return EXIT_FAILURE;
        }
        bool fail = false;
        if (FLAGS_source_file.empty()) {
            cerr << "ERROR: missing command-line flag -source_file" << endl;
            fail = true;
        }
        if (FLAGS_output_file.empty()) {
            cerr << "ERROR: missing command-line flag -output_file" << endl;
            fail = true;
        }
        if (fail) {
            return EXIT_FAILURE;
        }
        vector<string> includeDirs;
        vector<char> includeDirTokVector(FLAGS_include_dirs.begin(), FLAGS_include_dirs.end());
        char *tok = strtok(includeDirTokVector.data(), ";");
        while (tok) {
            includeDirs.emplace_back(tok);
            tok = strtok(nullptr, ";");
        }
        SyntaxTree main = Includer::instance->process(FLAGS_source_file, includeDirs);
        if (!FLAGS_schema_file.empty()) {
            SyntaxTree schema = Includer::instance->process(FLAGS_schema_file, includeDirs);
            if (!Validator::instance->validate(main, schema)) {
                cerr << "Device tree failed validation.  Exiting." << endl;
                return EXIT_FAILURE;
            }
        }
        if (!Emitter::instance->emit(main, FLAGS_output_file)) {
            cerr << "Failed to write device tree blob." << endl;
            return EXIT_FAILURE;
        }
    } catch (const exception &ex) {
        cerr << "ERROR: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
