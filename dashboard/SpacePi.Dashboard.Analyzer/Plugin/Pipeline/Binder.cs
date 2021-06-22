using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public abstract class Binder<TSource, TBinding, TClass> : BufferedPipeline<TClass> {
        private readonly List<TBinding> Bindings = new();
        [SuppressMessage("MicrosoftCodeAnalysisCorrectness", "RS1024:Compare symbols correctly", Justification = "Bug; Fixed in Microsoft.CodeAnalysis.Analyzers:3.3.3")]
        private readonly Dictionary<ISymbol, TSource> Lookup = new(SymbolEqualityComparer.Default);
        private GeneratorExecutionContext Context;

        public abstract ISymbol GetSymbol(TBinding binding);

        public abstract Diagnostic GenerateMissingDiagnostic(TBinding binding);

        public abstract void Bind(TSource source, TBinding binding);

        public override void Init(GeneratorExecutionContext ctx) {
            base.Init(ctx);
            Bindings.Clear();
            Lookup.Clear();
            Context = ctx;
        }

        public override IEnumerable<TClass> Finish() {
            foreach (TBinding binding in Bindings) {
                if (Lookup.TryGetValue(GetSymbol(binding), out TSource src)) {
                    Bind(src, binding);
                } else {
                    Context.ReportDiagnostic(GenerateMissingDiagnostic(binding));
                }
            }
            return base.Finish();
        }

        public void RegisterBinding(TBinding binding) => Bindings.Add(binding);

        public void RegisterSource(TSource source, ISymbol symbol) => Lookup[symbol] = source;
    }
}
