using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    public class BuildConfiguration {
        public readonly string SourceDir;
        public readonly string OutputDir;
        public readonly string StampFile;

        public void Clean() {
            if (Directory.Exists(OutputDir)) {
                Directory.Delete(OutputDir, true);
            }
        }

        public void Create() => Directory.CreateDirectory(OutputDir);

        public BuildConfiguration(AttributeData attr) {
            SourceDir = Path.GetDirectoryName(attr.ApplicationSyntaxReference.SyntaxTree.FilePath);
            OutputDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Analyzer)}/{nameof(Protobuf)}/{SourceDir.Substring(BuildConfig.CMAKE_SOURCE_DIR.Length + 1)}";
            StampFile = $"{OutputDir}/.stamp";
        }
    }
}
