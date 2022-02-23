#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/parser/SyntaxCell.hpp>
#include <spacepi/dtc/parser/SyntaxProperty.hpp>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::parser;

SyntaxTree::SyntaxTree(const SourceLocation &location, const string &name, const string &label) noexcept
    : location(location), name(name), label(label), properties { SyntaxProperty(SourceLocation(), "phandle", { SyntaxCell(SourceLocation(), (uint32_t)UINT32_MAX) }) } {
}

SyntaxTree::SyntaxTree(const SyntaxTree &other) noexcept {
    location = SourceLocation(other.getLocation());
    name = string(other.getName());
    label = string(other.getLabel());
    for(auto prop : other.getProperties()) {
        properties.push_back(SyntaxProperty(prop));
    }
    for(auto child : other.getChildren()) {
        children.push_back(SyntaxTree(child));
    }
    for(auto include : other.getIncludes()) {
        includes.push_back(string(include));
    }
    setPhandle(other.getPhandle());
}

bool SyntaxTree::operator==(const SyntaxTree &other) const noexcept {
    if (location != other.location || name != other.name || label != other.label) {
        return false;
    }
    vector<SyntaxTree>::const_iterator it1 = children.begin();
    vector<SyntaxTree>::const_iterator it2 = other.children.begin();
    while (it1 != children.end() && it2 != other.children.end()) {
        if (*it1++ != *it2++) {
            return false;
        }
    }
    vector<SyntaxProperty>::const_iterator it3 = properties.begin();
    vector<SyntaxProperty>::const_iterator it4 = other.properties.begin();
    while (it3 != properties.end() && it4 != other.properties.end()) {
        if (*it3++ != *it4++) {
            return false;
        }
    }
    vector<string>::const_iterator it5 = includes.begin();
    vector<string>::const_iterator it6 = other.includes.begin();
    while (it5 != includes.end() && it6 != other.includes.end()) {
        if (*it5++ != *it6++) {
            return false;
        }
    }
    return it1 == children.end() && it2 == other.children.end() && it3 == properties.end() && it4 == other.properties.end() && it5 == includes.end() && it6 == other.includes.end();
}

bool SyntaxTree::operator!=(const SyntaxTree &other) const noexcept {
    if (location != other.location || name != other.name || label != other.label) {
        return true;
    }
    vector<SyntaxTree>::const_iterator it1 = children.begin();
    vector<SyntaxTree>::const_iterator it2 = other.children.begin();
    while (it1 != children.end() && it2 != other.children.end()) {
        if (*it1++ != *it2++) {
            return true;
        }
    }
    vector<SyntaxProperty>::const_iterator it3 = properties.begin();
    vector<SyntaxProperty>::const_iterator it4 = other.properties.begin();
    while (it3 != properties.end() && it4 != other.properties.end()) {
        if (*it3++ != *it4++) {
            return true;
        }
    }
    vector<string>::const_iterator it5 = includes.begin();
    vector<string>::const_iterator it6 = other.includes.begin();
    while (it5 != includes.end() && it6 != other.includes.end()) {
        if (*it5++ != *it6++) {
            return true;
        }
    }
    return it1 != children.end() || it2 != other.children.end() || it3 != properties.end() || it4 != other.properties.end() || it5 != includes.end() || it6 != other.includes.end();
}

const SourceLocation &SyntaxTree::getLocation() const noexcept {
    return location;
}

const string &SyntaxTree::getName() const noexcept {
    return name;
}

const string &SyntaxTree::getLabel() const noexcept {
    return label;
}

void SyntaxTree::addChild(const SyntaxTree &node) noexcept {
    for(size_t i = 0; i < children.size(); i++) {
        if(children[i].getName() == node.getName()) {
            children[i] = node;
            return;
        }
    }

    children.push_back(node);
}

const vector<SyntaxTree> &SyntaxTree::getChildren() const noexcept {
    return children;
}

void SyntaxTree::addProperty(const SyntaxProperty &prop) noexcept {
    if (prop.getName() == "phandle") {
        properties[0] = prop;
    } else {
        for(size_t i = 0; i < properties.size(); i++) {
            if (properties[i].getName() == prop.getName()) {
                properties[i] = prop;
                return;
            }
        }

        properties.push_back(prop);
    }
}

const vector<SyntaxProperty> &SyntaxTree::getProperties() const noexcept {
    return properties;
}

void SyntaxTree::addInclude(const string &path) noexcept {
    includes.push_back(path);
}

const vector<string> &SyntaxTree::getIncludes() const noexcept {
    return includes;
}

void SyntaxTree::setPhandle(uint32_t phandle) noexcept {
    properties[0] = SyntaxProperty(SourceLocation(), "phandle", { SyntaxCell(SourceLocation(), phandle) });
}

uint32_t SyntaxTree::getPhandle() const noexcept {
    return properties[0].getCells().front().getValue();
}

ostream &parser::operator<<(ostream &os, const SyntaxTree &obj) noexcept {
    return os << "SyntaxTree(" << obj.getLocation() << ", \"" << obj.getName() << "\", \"" << obj.getLabel() << "\")";
}

void SyntaxTree::mergeTree(const SyntaxTree &other) noexcept {
    for (auto &child : other.getChildren()) {
        addChild(child);
    }
    for (const SyntaxProperty &prop : other.getProperties()) {
        if(prop.getName() == "phandle" && (int32_t)prop.getCells()[0].getValue() == -1) {
            // Don't propogate invalid phandle
        } else {
            addProperty(prop);
        }
    }
}
