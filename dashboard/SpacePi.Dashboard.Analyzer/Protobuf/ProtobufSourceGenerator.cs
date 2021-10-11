using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    /// <summary>
    /// The source generator which handles protobuf code
    /// </summary>
    [Generator]
    public class ProtobufSourceGenerator : SourceGenerator {
        /// <summary>
        /// Generates the protobuf code
        /// </summary>
        /// <param name="generator">The generator context</param>
        /// <param name="ctx">The compilation context</param>
        protected override void Generate(GeneratorExecutionContext generator, Context ctx) {
            ProtobufAnalyzer analyzer = new(ctx);
            analyzer.Clean();
            analyzer.CompileFiles(generator.AdditionalFiles);
            analyzer.GenerateSources(generator);
        }
    }
}
