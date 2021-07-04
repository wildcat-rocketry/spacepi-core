using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class SourceGenerator : ISourceGenerator {
        protected bool HasErrored { get; private set; }

        protected abstract IEnumerable<IEnumerable<Diagnostic>> Generate(GeneratorExecutionContext ctx);

        public void Execute(GeneratorExecutionContext ctx) {
            using DebugLog log = new(GetType().Name);
            Diagnostic ex = log.CatchAll(() => {
                HasErrored = false;
                foreach (IEnumerable<Diagnostic> diags in Generate(ctx)) {
                    foreach (Diagnostic diag in diags) {
                        ctx.ReportDiagnostic(diag);
                        if (diag.Severity == DiagnosticSeverity.Error) {
                            HasErrored = true;
                        }
                    }
                }
            });
            if (ex != null) {
                ctx.ReportDiagnostic(ex);
            }
        }

        public void Initialize(GeneratorInitializationContext ctx) {
        }
    }
}
