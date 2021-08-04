﻿using System;
using System.Collections.Generic;
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
        /// Creates a new configuration
        /// </summary>
        /// <param name="attr">The attribute declaring the configuration</param>
        public BuildConfiguration(AttributeData attr) {
            SourceDir = Path.GetDirectoryName(attr.ApplicationSyntaxReference.SyntaxTree.FilePath);
            OutputDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Analyzer)}/{nameof(Protobuf)}/{SourceDir.Substring(BuildConfig.CMAKE_SOURCE_DIR.Length + 1)}";
            StampFile = $"{OutputDir}/.stamp";
            SystemFiles = attr.ConstructorArguments.FirstOrDefault().Values.Select(s => s.Value.ToString()).ToArray();
        }
    }
}
