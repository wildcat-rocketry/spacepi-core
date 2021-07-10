using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class SourceGenerator : ISourceGenerator {
        protected abstract void Generate(GeneratorExecutionContext ctx, Diagnostics diags);

        public void Execute(GeneratorExecutionContext ctx) {
            Diagnostics diags = new();
            using DebugLog log = new(GetType().Name, diags);
            log.CatchAll(() => Generate(ctx, diags));
            foreach (Diagnostic diag in diags) {
                ctx.ReportDiagnostic(diag);
            }
        }

        public void Initialize(GeneratorInitializationContext ctx) {
        }
    }
}
