using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Diagnostics;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class ProtobufDiagnosticAnalyzer : DiagnosticAnalyzer<ProtobufAnalyzer> {
        protected override ProtobufAnalyzer CreateAnalyzer(Compilation compilation) => new(compilation);

        protected override void RegisterAnalyzer(DiagnosticRegistrar<ProtobufAnalyzer> registrar) => registrar.RegisterAdditionFileAction(RunAnalyzer);

        private IEnumerable<IEnumerable<Diagnostic>> RunAnalyzer(AdditionalFileAnalysisContext context, ProtobufAnalyzer analyzer) {
            yield return analyzer.CompileFiles(new[] { context.AdditionalFile });
        }
    }
}
