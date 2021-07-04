using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Diagnostics;

namespace SpacePi.Dashboard.Analyzer {
    public class DiagnosticRegistrar<TContext> {
        private readonly TContext Context;
        private readonly CompilationStartAnalysisContext Analysis;

        public void RegisterSymbolAction(Func<SymbolAnalysisContext, TContext, IEnumerable<IEnumerable<Diagnostic>>> action, params SymbolKind[] kinds) =>
            Analysis.RegisterSymbolAction(c => {
                foreach (IEnumerable<Diagnostic> diags in action(c, Context)) {
                    foreach (Diagnostic diag in diags) {
                        c.ReportDiagnostic(diag);
                    }
                }
            }, kinds);

        public void RegisterAdditionFileAction(Func<AdditionalFileAnalysisContext, TContext, IEnumerable<IEnumerable<Diagnostic>>> action) =>
            Analysis.RegisterAdditionalFileAction(c => {
                foreach (IEnumerable<Diagnostic> diags in action(c, Context)) {
                    foreach (Diagnostic diag in diags) {
                        c.ReportDiagnostic(diag);
                    }
                }
            });

        [SuppressMessage("MicrosoftCodeAnalysisPerformance", "RS1012:Start action has no registered actions", Justification = "Registration happens elsewhere")]
        public DiagnosticRegistrar(TContext ctx, CompilationStartAnalysisContext analysis) {
            Context = ctx;
            Analysis = analysis;
        }
    }
}
