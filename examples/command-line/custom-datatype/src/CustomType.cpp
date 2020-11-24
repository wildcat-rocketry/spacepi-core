#include <exception>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <spacepi/example/CustomType.hpp>
#include <spacepi/util/CommandInternals.hpp>

using namespace std;
using namespace spacepi::example;
using namespace spacepi::util::detail;

CustomType::CustomType(int val) noexcept : val(val) {
}

int CustomType::getVal() const noexcept {
    return val;
}

ostream &operator <<(ostream &os, const CustomType &obj) {
    os << "(CustomType " << obj.getVal() << ")";
    return os;
}

template <>
pair<vector<string>::const_iterator, string> CommandParser<CustomType>::parse(const vector<string> &args, const vector<string>::const_iterator &start) noexcept {
    pair<string, pair<vector<string>::const_iterator, string>> res = parseNormal(args, start);
    if (!res.first.empty()) {
        try {
            size_t idx = 0;
            int i = stoi(res.first, &idx);
            if (idx != res.first.length() || i < 42) {
                throw exception();
            }
            var = CustomType(i);
            valid = true;
        } catch (const exception &) {
            return make_pair(res.second.first, "Invalid argument for --" + name + " (must be >= 42)");
        }
    }
    return res.second;
}

template <>
string CommandParser<CustomType>::example() const noexcept {
    return "--" + name + "=42";
}
