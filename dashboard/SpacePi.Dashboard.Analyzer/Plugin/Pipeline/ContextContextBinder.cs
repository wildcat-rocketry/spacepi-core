using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class ContextContextBinder : Binder<ContextClass, BoundPlugin<ContextClass, ContextClass>, ContextClass> {
        private ITypeSymbol BindContextAttribute;

        public override ISymbol GetSymbol(BoundPlugin<ContextClass, ContextClass> binding) => binding.TargetClassSymbol;

        public override void Bind(ContextClass source, BoundPlugin<ContextClass, ContextClass> binding) => binding.TargetClass = source;

        public override void Init(GeneratorExecutionContext ctx) {
            base.Init(ctx);
            BindContextAttribute = ctx.Compilation.GetTypeByMetadataName("SpacePi.Dashboard.API.BindContextAttribute");
        }

        public override void Process(ContextClass context) {
            foreach (ISymbol sym in context.Symbol.GetMembers().Where(m => (m.Kind == SymbolKind.Field || m.Kind == SymbolKind.Property)
                && m.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, BindContextAttribute))
                && m.DeclaredAccessibility == Accessibility.Public
                && !m.IsAbstract
                && !m.IsStatic
                && (m.Kind != SymbolKind.Property || !((IPropertySymbol) m).IsReadOnly)
                && (m.Kind != SymbolKind.Field || !((IFieldSymbol) m).IsReadOnly))) {
                BoundPlugin<ContextClass, ContextClass> binding = new() {
                    Parent = context,
                    FieldName = sym.Name,
                    TargetClassSymbol = sym.Kind == SymbolKind.Property ? ((IPropertySymbol) sym).Type : ((IFieldSymbol) sym).Type
                };
                context.BoundContexts.Add(binding);
                RegisterBinding(binding);
            }
            RegisterSource(context, context.Symbol);
        }
    }
}
