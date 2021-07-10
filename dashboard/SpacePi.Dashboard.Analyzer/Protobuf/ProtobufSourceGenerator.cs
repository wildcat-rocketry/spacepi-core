using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    [Generator]
    public class ProtobufSourceGenerator : SourceGenerator {
        protected override void Generate(GeneratorExecutionContext ctx, Diagnostics diags) {
            ProtobufAnalyzer analyzer = new(ctx.Compilation, diags);
            analyzer.Clean();
            analyzer.CompileFiles(ctx.AdditionalFiles);
            analyzer.GenerateSources(ctx);
        }
    }
}
