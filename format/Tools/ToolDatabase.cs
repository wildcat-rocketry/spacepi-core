using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Format.Tools {
    public class ToolDatabase : IDisposable {
        private static readonly Dictionary<string, Func<IFormatTool>> Tools = new() {
            { "clang", () => new ClangFormatTool() },
            { "dotnet", () => new DotnetFormatTool() },
            { "regex", () => new RegexTool() }
        };

        private readonly Dictionary<string, IFormatTool> ToolInstances = new();
        private bool Disposed;

        public IFormatTool this[string name] {
            get {
                Func<IFormatTool> ctor;
                if (ToolInstances.TryGetValue(name, out IFormatTool tool)) {
                    return tool;
                } else if (!Tools.TryGetValue(name, out ctor)) {
                    ctor = () => new NullFormatTool(name);
                }
                tool = ctor();
                ToolInstances.Add(name, tool);
                return tool;
            }
        }

        protected virtual void Dispose(bool disposing) {
            if (!Disposed) {
                if (disposing) {
                    foreach (IFormatTool tool in ToolInstances.Values) {
                        tool.Dispose();
                    }
                }
                Disposed = true;
            }
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
