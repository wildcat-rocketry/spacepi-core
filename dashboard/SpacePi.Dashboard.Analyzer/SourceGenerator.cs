using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class SourceGenerator : ISourceGenerator {
        protected abstract void Generate(GeneratorExecutionContext generator, Context ctx);

        public void Execute(GeneratorExecutionContext generator) {
            Diagnostics diags = new();
            using DebugLog log = new(GetType().Name, diags);
            log.CatchAll(() => Generate(generator, new(generator.Compilation, diags)));
            foreach (Diagnostic diag in diags) {
                generator.ReportDiagnostic(diag);
            }
        }

        public void Initialize(GeneratorInitializationContext ctx) {
        }
    }
}
