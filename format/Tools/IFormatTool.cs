using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Format.Tools {
    public interface IFormatTool : IDisposable {
        string ConfigFileName { get; }
        IEnumerable<string> FileExtensions { get; }

        bool Format(IEnumerable<string> codeFiles, string formatFile, bool write);
    }
}
