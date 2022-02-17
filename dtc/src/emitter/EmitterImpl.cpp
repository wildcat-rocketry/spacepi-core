#include <fstream>
#include <string>

#include <spacepi/dtc/emitter/DTBFile.hpp>
#include <spacepi/dtc/emitter/DTBHeader.hpp>
#include <spacepi/dtc/emitter/EmitterImpl.hpp>
#include <spacepi/dtc/emitter/LabelMapping.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

using namespace std;
using namespace spacepi::dtc::emitter;
using namespace spacepi::dtc::parser;

bool EmitterImpl::emit(const SyntaxTree &tree, const string &filename) const noexcept {
    DTBFile file(filename);
    file.seek(0); // Making sure

    LabelMapping labels;
    DTBHeader header;

    uint32_t pos;

    // Write empty header to make room
    header.write_header(file);

    // Write memory reservation block. Currently the array terminator of two zeros since it's unused.
    header.set_memory_reservations_pos(file.position());
    file.write((uint64_t)0);
    file.write((uint64_t)0);

    pos = file.position();
    header.set_struct_pos(pos);
    write_node(file, labels, tree, tree);
    file.write(FDT_END);
    header.set_struct_size((uint32_t)file.position() - pos);
    pos = file.position();
    header.set_strings_pos(pos);
    labels.write_labels(file);
    header.set_strings_size((uint32_t)file.position() - pos);

    // Rewrite header now that we have the data
    file.seek(0);
    header.write_header(file);

    file.close();

    return true;
}

bool EmitterImpl::write_node(DTBFile &file, LabelMapping &labels, const SyntaxTree &base, const SyntaxTree &tree) const noexcept {
    file.write(FDT_BEGIN_NODE);
    file.write(tree.getName());
    file.write((uint8_t)'\0');
    
    while(file.position() % 4 != 0) {
        file.write((uint8_t)0);
    }

    for(auto const& property : tree.getProperties()) {
        write_property(file, labels, base, property);
    }

    for(auto const& child : tree.getChildren()) {
        write_node(file, labels, base, child);
    }

    file.write(FDT_END_NODE);

    return true;
}

bool EmitterImpl::write_property(DTBFile &file, LabelMapping &labels, const SyntaxTree &base, const SyntaxProperty &property) const noexcept {
    file.write(FDT_PROP);
    if(property.isLabel()){
        string addr = find_label(base, property.getLabel()).first;
        if(addr == "") return false;
        file.write(addr.length() + 1);
        file.write(labels.add_label(property.getName()));
        file.write(addr);
        file.write((uint8_t)'\0');
    } else if (property.isString()){
        file.write(property.getBytes().size() + 1);
        file.write(labels.add_label(property.getName()));
        file.write(property.getBytes());
        file.write((uint8_t)'\0');
    } else {
        file.write(property.getCells().size() * 4);
        file.write(labels.add_label(property.getName()));
        for(const SyntaxCell &cell : property.getCells()){
            auto search = find_label(base, cell.getLabel());
            if(search.first == "") return false;

            if(cell.isLabel()){
                file.write(search.second.getPhandle());
            } else {
                file.write(cell.getValue());
            }
        }
    }

    // Pad until on 32 bit boundary
    while(file.position() % 4 != 0){
        file.write((uint8_t)0);
    }

    return true;
}

pair<string, const SyntaxTree &> EmitterImpl::find_label(const SyntaxTree &tree, const string &label, const string &addr) noexcept {
    if (tree.getLabel() == label) {
        return pair<string, const SyntaxTree &>(addr, tree);
    }

    for (const SyntaxTree &child: tree.getChildren()) {
        return find_label(child, label, addr + "/" + child.getName());
    }

    return pair<string, const SyntaxTree &>("", tree);
}