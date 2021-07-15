using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    [Generator]
    public class ProtobufSourceGenerator : SourceGenerator {
        protected override void Generate(GeneratorExecutionContext generator, Context ctx) {
            ProtobufAnalyzer analyzer = new(ctx);
            analyzer.Clean();
            analyzer.CompileFiles(generator.AdditionalFiles);
            analyzer.GenerateSources(generator);
        }
    }
}
