using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Diagnostics;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class DiagnosticAnalyzer<TAnalyzer> : DiagnosticAnalyzer where TAnalyzer : Analyzer {
        private static readonly ImmutableArray<DiagnosticDescriptor> DiagnosticsArray = ImmutableArray.Create(Diag.All.ToArray());

        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics => DiagnosticsArray;

        protected abstract TAnalyzer CreateAnalyzer(Compilation compilation);

        protected abstract void RegisterAnalyzer(DiagnosticRegistrar<TAnalyzer> registrar);

        public override void Initialize(AnalysisContext ctx) {
            ctx.EnableConcurrentExecution();
            ctx.ConfigureGeneratedCodeAnalysis(GeneratedCodeAnalysisFlags.Analyze | GeneratedCodeAnalysisFlags.ReportDiagnostics);
            ctx.RegisterCompilationStartAction(c => RegisterAnalyzer(new(CreateAnalyzer(c.Compilation), c)));
        }
    }
}
