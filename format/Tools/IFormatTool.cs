using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Format.Tools {
    public interface IFormatTool : IDisposable {
        bool Format(IEnumerable<string> codeFiles, string formatFile, bool write);
    }
}
