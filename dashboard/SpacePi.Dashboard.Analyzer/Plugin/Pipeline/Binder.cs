using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    abstract class Binder<TSource, TBinding, TClass> : BufferedPipeline<TClass> {
        private readonly List<TBinding> Bindings = new();
        [SuppressMessage("MicrosoftCodeAnalysisCorrectness", "RS1024:Compare symbols correctly", Justification = "Bug; Fixed in Microsoft.CodeAnalysis.Analyzers:3.3.3")]
        private readonly Dictionary<ISymbol, TSource> Lookup = new(SymbolEqualityComparer.Default);

        public abstract ISymbol GetSymbol(TBinding binding);

        public abstract void Bind(TSource source, TBinding binding);

        public override void Init(GeneratorExecutionContext ctx) {
            base.Init(ctx);
            Bindings.Clear();
            Lookup.Clear();
        }

        public override IEnumerable<TClass> Finish() {
            foreach (TBinding binding in Bindings) {
                Bind(Lookup[GetSymbol(binding)], binding);
            }
            return base.Finish();
        }

        public void RegisterBinding(TBinding binding) => Bindings.Add(binding);

        public void RegisterSource(TSource source, ISymbol symbol) => Lookup[symbol] = source;
    }
}
