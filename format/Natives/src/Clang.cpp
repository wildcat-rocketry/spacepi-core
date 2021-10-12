#include <memory>
#include <string>
#include <vector>
#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SMLoc.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/VirtualFileSystem.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticIDs.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/FileEntry.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/FileSystemOptions.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Format/Format.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/Core/Replacement.h>
#include <SpacePi/Format/Natives/Clang.hpp>

using namespace std;
using namespace llvm;
using namespace llvm::vfs;
using namespace clang;
using namespace clang::format;
using namespace clang::tooling;

static std::unique_ptr<InitLLVM> llvmInstance;

void SpacePi_Format_Natives_Clang_Construct() {
    static const char *_argv[] = { "SpacePi.Format.Natives" };
    int argc = 1;
    const char **argv = _argv;
    llvmInstance.reset(new InitLLVM(argc, argv));
}

void *SpacePi_Format_Natives_Clang_LoadStyle(const char *_formatFile) {
    StringRef formatFile = _formatFile;
    Expected<FormatStyle> style = getStyle("file", (formatFile.take_front(formatFile.find_last_of('/')) + "/.dummy").str(), "none");
    if (!style) {
        errs() << toString(style.takeError()) << "\n";
        return nullptr;
    }
    return new FormatStyle(*style);
}

bool SpacePi_Format_Natives_Clang_Format(void *_style, const char *codeFile, bool write) {
    FormatStyle *style = (FormatStyle *) _style;
    if (!style) {
        return false;
    }
    ErrorOr<unique_ptr<MemoryBuffer>> codeStream = MemoryBuffer::getFileAsStream(codeFile);
    if (!codeStream) {
        errs() << codeStream.getError().message() << "\n";
        return false;
    }
    if ((*codeStream)->getBufferSize() == 0) {
        return true;
    }
    StringRef code = (*codeStream)->getBuffer();
    const char *invalidBOM = SrcMgr::ContentCache::getInvalidBOM(code);
    if (invalidBOM) {
        errs() << "error: encoding with unsupported byte order mark \""
               << invalidBOM << "\" detected\n";
        return false;
    }
    vector<Range> ranges;
    ranges.emplace_back(0, (int) (*codeStream)->getBufferSize());
    Replacements repl = sortIncludes(*style, code, ranges, codeFile);
    Expected<string> formattedCode = applyAllReplacements(code, repl);
    if (!formattedCode) {
        errs() << toString(formattedCode.takeError()) << "\n";
        return false;
    }
    ranges = calculateRangesAfterReplacements(repl, ranges);
    FormattingAttemptStatus status;
    repl = repl.merge(reformat(*style, *formattedCode, ranges, codeFile, &status));
    if (write) {
        IntrusiveRefCntPtr<InMemoryFileSystem> fs(new InMemoryFileSystem());
        FileManager files(FileSystemOptions(), fs);
        DiagnosticsEngine diags(IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs()), new DiagnosticOptions());
        SourceManager sources(diags, files);
        fs->addFileNoOwn(codeFile, 0, **codeStream);
        Optional<FileEntryRef> file = files.getOptionalFileRef(codeFile);
        FileID fid = sources.createFileID(*file, SourceLocation(), SrcMgr::C_User);
        Rewriter rewriter(sources, LangOptions());
        applyAllReplacements(repl, rewriter);
        if (rewriter.overwriteChangedFiles()) {
            errs() << "error overwriting changed files\n";
            return false;
        }
        outs() << "Formatted code file '" << codeFile << "'.\n";
    } else if (!repl.empty()) {
        SourceMgr sources;
        const char *buf = (*codeStream)->getBufferStart();
        sources.AddNewSourceBuffer(MemoryBuffer::getMemBuffer(buf, codeFile), SMLoc());
        for (Replacements::const_iterator it = repl.begin(); it != repl.end(); ++it) {
            SMDiagnostic diag = sources.GetMessage(SMLoc::getFromPointer(buf + it->getOffset()), SourceMgr::DiagKind::DK_Error, "code should be formatted");
            diag.print(nullptr, errs());
        }
        return false;
    }
    return true;
}

void SpacePi_Format_Natives_Clang_FreeStyle(void *style) {
    if (!style) {
        return;
    }
    delete (FormatStyle *) style;
}

void SpacePi_Format_Natives_Clang_Dispose() {
    llvmInstance.reset();
}
