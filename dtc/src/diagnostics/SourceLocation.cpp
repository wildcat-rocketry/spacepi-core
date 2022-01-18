#include <limits>
#include <memory>
#include <ostream>
#include <spacepi/dtc/diagnostics/SourceFile.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;

SourceLocation::SourceLocation(const shared_ptr<SourceFile> &file, int lineNo, int fromCol, int toCol) noexcept
    : file(file), lineNo(lineNo), fromCol(fromCol), toCol(toCol) {
}

bool SourceLocation::operator==(const SourceLocation &other) const noexcept {
    return *file == *other.file && lineNo == other.lineNo && fromCol == other.fromCol && toCol == other.toCol;
}

bool SourceLocation::operator!=(const SourceLocation &other) const noexcept {
    return *file != *other.file || lineNo != other.lineNo || fromCol != other.fromCol || toCol != other.toCol;
}

SourceLocation::operator bool() const noexcept {
    return file && lineNo > 0 && fromCol > 0 && toCol > 0;
}

const shared_ptr<SourceFile> &SourceLocation::getFile() const noexcept {
    return file;
}

int SourceLocation::getLineNumber() const noexcept {
    return lineNo;
}

int SourceLocation::getColumnStart() const noexcept {
    return fromCol;
}

int SourceLocation::getColumnEnd() const noexcept {
    return toCol;
}

ostream &diagnostics::operator<<(ostream &os, const SourceLocation &obj) noexcept {
    os << "SourceLocation(";
    if (obj.getFile()) {
        os << *obj.getFile();
    } else {
        os << "nullptr";
    }
    return os << ", " << obj.getLineNumber() << ", " << obj.getColumnStart() << ", " << obj.getColumnEnd() << ")";
}
