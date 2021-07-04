using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public record ContextClass : IEquatable<ContextClass>, IClassBindingTarget, IPluginBindable, IContextBindable {
        public ITypeSymbol Symbol { get; set; }
        public int Index { get; set; } = -1;
        public PluginClass OwnerPlugin { get; set; }
        public ITypeSymbol PluginInterface { get; set; }
        public List<Binding<PluginClass>> PluginBindings { get; set; } = new();
        public List<Binding<ContextClass>> ContextBindings { get; set; } = new();

        public virtual bool Equals(ContextClass other) => other != null && SymbolEqualityComparer.Default.Equals(Symbol, other.Symbol);

        public override int GetHashCode() => SymbolEqualityComparer.Default.GetHashCode(Symbol);

        public void CopyTo(ContextClass other) {
            other.Symbol = Symbol;
            other.Index = Index;
            other.OwnerPlugin = OwnerPlugin;
            other.PluginInterface = PluginInterface;
            other.PluginBindings = PluginBindings;
            other.ContextBindings = ContextBindings;
        }
    }
}
