//#define SOURCE_GENERATOR_ATTACH_DEBUGGER

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// A base class for source generators
    /// </summary>
    public abstract class SourceGenerator : ISourceGenerator {
        /// <summary>
        /// Generates the generated sources
        /// </summary>
        /// <param name="generator">The generator context</param>
        /// <param name="ctx">The compilation context</param>
        protected abstract void Generate(GeneratorExecutionContext generator, Context ctx);

        /// <summary>
        /// Runs the source generator
        /// </summary>
        /// <param name="generator">The generator context</param>
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

        /// <summary>
        /// Initializes the source generator
        /// </summary>
        /// <param name="ctx">The generator context</param>
        public void Initialize(GeneratorInitializationContext ctx) {
        }
    }
}
