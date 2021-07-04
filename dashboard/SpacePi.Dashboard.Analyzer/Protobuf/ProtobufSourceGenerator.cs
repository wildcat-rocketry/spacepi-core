using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    [Generator]
    public class ProtobufSourceGenerator : SourceGenerator {
        protected override IEnumerable<IEnumerable<Diagnostic>> Generate(GeneratorExecutionContext ctx) {
            ProtobufAnalyzer analyzer = new(ctx.Compilation);
            analyzer.Clean();
            yield return analyzer.CompileFiles(ctx.AdditionalFiles);
            analyzer.GenerateSources(ctx);
        }
    }
}
