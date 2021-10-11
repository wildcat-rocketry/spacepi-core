using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Format.Tools {
    public class NullFormatTool : IFormatTool {
        private readonly string Name;

        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) {
            Console.Error.WriteLine($"Error: unknown FormatTool '{Name}'");
            return false;
        }

        protected virtual void Dispose(bool disposing) {
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public NullFormatTool(string name) => Name = name;
    }
}
