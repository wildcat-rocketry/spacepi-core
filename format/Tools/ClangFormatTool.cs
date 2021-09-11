using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace SpacePi.Format.Tools {
    public class ClangFormatTool : IFormatTool {
        private bool disposed;

        [DllImport("spacepi-clang-format-interface")]
        private static extern void NativeConstruct();

        [DllImport("spacepi-clang-format-interface")]
        private static extern IntPtr LoadStyle(string formatFile);

        [DllImport("spacepi-clang-format-interface")]
        private static extern bool Format(IntPtr style, string codeFile, bool write);

        [DllImport("spacepi-clang-format-interface")]
        private static extern void FreeStyle(IntPtr style);

        [DllImport("spacepi-clang-format-interface")]
        private static extern void NativeDispose();

        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) {
            IntPtr style = LoadStyle(formatFile);
            try {
                return codeFiles.Sum(f => Format(style, f, write) ? 0 : 1) == 0;
            } finally {
                FreeStyle(style);
            }
        }

        protected virtual void Dispose(bool disposing) {
            if (!disposed) {
                NativeDispose();
                disposed = true;
            }
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public ClangFormatTool() => NativeConstruct();

        ~ClangFormatTool() => Dispose(false);
    }
}
