using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    /// <summary>
    /// A configuration with which to build protobuf code
    /// </summary>
    public class BuildConfiguration {
        /// <summary>
        /// The root of the directory in which to find sources
        /// </summary>
        public readonly string SourceDir;
        /// <summary>
        /// The directory in which to output files
        /// </summary>
        public readonly string OutputDir;
        /// <summary>
        /// The file used to determine when source files are out-of-date
        /// </summary>
        public readonly string StampFile;
        /// <summary>
        /// A list of system files to include in the compilation
        /// </summary>
        public readonly string[] SystemFiles;

        /// <summary>
        /// Cleans the configuration
        /// </summary>
        public void Clean() {
            if (Directory.Exists(OutputDir)) {
                Directory.Delete(OutputDir, true);
            }
        }

        /// <summary>
        /// Creates a new directory for the configuration output
        /// </summary>
        public void Create() => Directory.CreateDirectory(OutputDir);

        /// <summary>
        /// Generates a string representation of the BuildConfiguration
        /// </summary>
        /// <returns>The string</returns>
        public override string ToString() {
            string sysFiles = string.Join(",", SystemFiles.Select(s => $"\"{s}\""));
            return $"BuildConfiguration(\"{SourceDir}\", \"{OutputDir}\", \"{StampFile}\", [ {sysFiles} ])";
        }

        /// <summary>
        /// Creates a new configuration
        /// </summary>
        /// <param name="attr">The attribute declaring the configuration</param>
        [SuppressMessage("Style", "IDE0057:Use range operator", Justification = "Cannot use ranges on NETStandard 2.0")]
        public BuildConfiguration(AttributeData attr) {
            SourceDir = Path.GetDirectoryName(attr.ApplicationSyntaxReference.SyntaxTree.FilePath);
            OutputDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Analyzer)}/{nameof(Protobuf)}/{SourceDir.Substring(BuildConfig.CMAKE_SOURCE_DIR.Length + 1)}";
            StampFile = $"{OutputDir}/.stamp";
            SystemFiles = attr.ConstructorArguments.FirstOrDefault().Values.Select(s => s.Value.ToString()).ToArray();
        }
    }
}
