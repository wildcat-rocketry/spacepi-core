//#define SOURCE_GENERATOR_ATTACH_DEBUGGER

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class SourceGenerator : ISourceGenerator {
        protected abstract void Generate(GeneratorExecutionContext generator, Context ctx);

        public void Execute(GeneratorExecutionContext generator) {
#if SOURCE_GENERATOR_ATTACH_DEBUGGER
            if (!Debugger.IsAttached) {
                Debugger.Launch();
            }
#endif
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
