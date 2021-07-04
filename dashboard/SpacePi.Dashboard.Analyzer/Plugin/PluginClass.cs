using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public record PluginClass : IEquatable<PluginClass>, IClassBindingTarget, IPluginBindable {
        public ITypeSymbol Symbol { get; set; }
        public int Index { get; set; } = -1;
        public int Priority { get; set; }
        public Dictionary<ContextClass, ContextClass> Contexts { get; set; } = new();
        public List<Binding<PluginClass>> PluginBindings { get; set; } = new();

        public virtual bool Equals(PluginClass other) => other != null && SymbolEqualityComparer.Default.Equals(Symbol, other.Symbol);

        public override int GetHashCode() => SymbolEqualityComparer.Default.GetHashCode(Symbol);

        public void CopyTo(PluginClass other) {
            other.Symbol = Symbol;
            other.Index = Index;
            other.Priority = Priority;
            other.Contexts = Contexts;
            other.PluginBindings = PluginBindings;
        }
    }
}
