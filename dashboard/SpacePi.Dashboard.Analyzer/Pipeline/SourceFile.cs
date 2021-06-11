using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    record SourceFile {
        public string FileBaseName;
        public StringBuilder Contents = new();
    }
}
