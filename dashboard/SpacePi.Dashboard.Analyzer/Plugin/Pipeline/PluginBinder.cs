using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public abstract class PluginBinder<TSource, TClass> : Binder<TSource, BoundPlugin<TSource, PluginClass>, TClass> {
        private ITypeSymbol BindPluginAttribute;

        public abstract PluginClass GetPlugin(TSource source);

        public abstract void RegisterBinding(TSource source, BoundPlugin<TSource, PluginClass> binding);

        public override ISymbol GetSymbol(BoundPlugin<TSource, PluginClass> binding) => binding.TargetClassSymbol;

        public override void Bind(TSource source, BoundPlugin<TSource, PluginClass> binding) => binding.TargetClass = GetPlugin(source);

        public override void Init(GeneratorExecutionContext ctx) {
            base.Init(ctx);
            BindPluginAttribute = ctx.Compilation.GetTypeByMetadataName(Types.BindPluginAttribute);
        }

        public void ProcessSource(TSource source, ITypeSymbol symbol) {
            foreach (ISymbol sym in symbol.GetMembers().Where(m => (m.Kind == SymbolKind.Field || m.Kind == SymbolKind.Property)
                && m.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, BindPluginAttribute))
                && m.DeclaredAccessibility == Accessibility.Public
                && !m.IsAbstract
                && !m.IsStatic
                && (m.Kind != SymbolKind.Property || !((IPropertySymbol)m).IsReadOnly)
                && (m.Kind != SymbolKind.Field || !((IFieldSymbol)m).IsReadOnly))) {
                BoundPlugin<TSource, PluginClass> binding = new() {
                    Parent = source,
                    FieldName = sym.Name,
                    TargetClassSymbol = sym.Kind == SymbolKind.Property ? ((IPropertySymbol)sym).Type : ((IFieldSymbol)sym).Type
                };
                RegisterBinding(source, binding);
                RegisterBinding(binding);
            }
            RegisterSource(source, symbol);
        }
    }
}
