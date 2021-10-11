using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SpacePi.Format.Natives;

namespace SpacePi.Format.Tools {
    public class ClangFormatTool : IFormatTool {
        private bool Disposed;

        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) {
            IntPtr style = Clang.LoadStyle(formatFile);
            try {
                return codeFiles.Sum(f => Clang.Format(style, f, write) ? 0 : 1) == 0;
            } finally {
                Clang.FreeStyle(style);
            }
        }

        protected virtual void Dispose(bool disposing) {
            if (!Disposed) {
                Clang.Dispose();
                Disposed = true;
            }
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public ClangFormatTool() => Clang.Construct();

        ~ClangFormatTool() => Dispose(false);
    }
}
