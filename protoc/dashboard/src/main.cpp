#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <google/protobuf/compiler/plugin.pb.h>

using namespace std;
using namespace google::protobuf::compiler;

int main(int argc, char **argv) noexcept {
    CodeGeneratorRequest req;
    if (!req.ParseFromIstream(&cin)) {
        cerr << "unable to parse input CodeGeneratorRequest" << endl;
        return EXIT_FAILURE;
    }
    CodeGeneratorResponse res;
    if (req.parameter().empty()) {
        res.set_error("parameter must be set to output filename");
    } else {
        CodeGeneratorResponse_File *file = res.add_file();
        vector<uint8_t> raw;
        raw.resize(req.ByteSizeLong());
        if (!req.SerializeToArray(raw.data(), (int) raw.size())) {
            cerr << "unable to serialize input CodeGeneratorRequest" << endl;
            return EXIT_FAILURE;
        }
        file->set_name(req.parameter());
        vector<char> out;
        static const char prefix[] = "-----BEGIN SPACEPI DASHBOARD BUNDLE-----\n";
        static const char postfix[] = "-----END SPACEPI DASHBOARD BUNDLE-----\n";
        out.reserve(sizeof(prefix) - 1 + (raw.size() + 2) / 3 * 4 + (raw.size() + 59) / 60 + sizeof(postfix));
        out.resize(sizeof(prefix) - 1);
        memcpy(out.data(), prefix, sizeof(prefix) - 1);
        int width = 0;
        static const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        for (vector<uint8_t>::const_iterator it = raw.begin(); it != raw.end();) {
            uint32_t word = 0;
            if (it != raw.end())
                word |= 0x01000000 | (*it++ << 16);
            if (it != raw.end())
                word |= 0x02000000 | (*it++ << 8);
            if (it != raw.end())
                word |= 0x04000000 | (*it++ << 0);
            out.push_back(word & 0x01000000 ? alphabet[(word >> 18) & 0x3F] : '=');
            out.push_back(word & 0x03000000 ? alphabet[(word >> 12) & 0x3F] : '=');
            out.push_back(word & 0x06000000 ? alphabet[(word >> 6) & 0x3F] : '=');
            out.push_back(word & 0x04000000 ? alphabet[(word >> 0) & 0x3F] : '=');
            if (++width == 20) {
                width = 0;
                out.push_back('\n');
            }
        }
        if (width != 0) {
            out.push_back('\n');
        }
        char *back = out.data() + out.size();
        out.resize(out.capacity());
        strcpy(back, postfix);
        file->set_content(out.data());
    }
    cout << res.SerializeAsString() << flush;
    return EXIT_SUCCESS;
}
