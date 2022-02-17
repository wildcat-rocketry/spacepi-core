#include <string>
#include <unordered_map>

#include <spacepi/dtc/emitter/DTBFile.hpp>
#include <spacepi/dtc/emitter/LabelMapping.hpp>

using namespace std;
using namespace spacepi::dtc::emitter;

LabelMapping::LabelMapping() noexcept
    : labels()
{
}

uint32_t LabelMapping::add_label(const string &label) noexcept {
    auto it = labels.find(label);
    if (it != labels.end()) {
        return it->second;
    }
    
    labels.insert(make_pair(label, strings_pos));
    uint32_t address = strings_pos;

    strings_pos += label.size() + 1;

    return address;
}

void LabelMapping::write_labels(DTBFile &file) const noexcept {
    uint32_t count;
    size_t pos = 0;
    for(count = labels.size(); count > 0; count--) {
        for(auto const& x : labels) {
            if (x.second == pos) {
                file.write(x.first);
                file.write((uint8_t)'\0');
                pos += x.first.size() + 1;
                break;
            }
        }
    }
}