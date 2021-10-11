using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis.Tools;

namespace SpacePi.Format.Tools {
    public class DotnetFormatTool : IFormatTool {
        private readonly MethodInfo MainMethod;

        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) => !codeFiles.Any() || 0 == (int) MainMethod.Invoke(null, new object[] { new[] {
                Path.GetDirectoryName(formatFile),
                "--folder",
                "--fix-whitespace",
                "--include"
            }.Concat(codeFiles).Concat(write ? Array.Empty<string>() : new[] { "--check" }).ToArray() });

        protected virtual void Dispose(bool disposing) {
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public DotnetFormatTool() => MainMethod = typeof(FormattedFile).Assembly.EntryPoint;
    }
}
