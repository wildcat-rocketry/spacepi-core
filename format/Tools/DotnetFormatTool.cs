using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Format.Tools {
    public class DotnetFormatTool : IFormatTool {
        private readonly MethodInfo MainMethod;
        private bool disposed;

        public string ConfigFileName => ".editorconfig";

        public IEnumerable<string> FileExtensions => new[] { "cs" };

        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) {
            Task<int> task = (Task<int>) MainMethod.Invoke(null, new[] {
                Path.GetDirectoryName(formatFile),
                "--folder",
                "--fix-whitespace",
                "--fix-style",
                "--fix-analyzers",
                "--include"
            }.Concat(codeFiles).Concat(write ? Array.Empty<string>() : new[] { "--check" }).ToArray());
            task.Wait();
            return task.IsCompletedSuccessfully && task.Result == 0;
        }

        protected virtual void Dispose(bool disposing) {
            if (!disposed) {
                disposed = true;
            }
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public DotnetFormatTool() => MainMethod = Type.GetType("Microsoft.CodeAnalysis.Tools.Program").GetMethod("Main", BindingFlags.NonPublic);
    }
}
