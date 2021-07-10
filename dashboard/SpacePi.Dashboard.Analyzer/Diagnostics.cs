using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer {
    public partial class Diagnostics {
        ////////////////////////////////////////////////////////////////////////
        // General Diagnostics
        public readonly D AnalyzerException = new(Error, 0,
            "Analyzer encountered an exception",
            "Analyzer encountered an unhandled {0}: {1}");
        public readonly D APITypeMissing = new(Error, 1,
            "Missing Analyzer API type",
            "Analyzer API type {0} was not found in the current compilation context");

        ////////////////////////////////////////////////////////////////////////
        // Protobuf Diagnostics
        public readonly D CMakeBuildStatus = new(Info, 100,
            "CMake build message",
            "{0}");
        public readonly D CMakeBuildError = new(Error, 101,
            "CMake build error",
            "{0}");
        public readonly D ProtocBuildStatus = new(Info, 102,
            "CMake build message",
            "{0}");
        public readonly D ProtocBuildError = new(Error, 103,
            "CMake build error",
            "{0}");
    }
}
