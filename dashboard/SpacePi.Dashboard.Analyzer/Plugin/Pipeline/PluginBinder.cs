using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PluginBinder : BufferedPipeline<PluginClass> {
        private readonly List<BoundPlugin> Bindings = new();
        private readonly Dictionary<ITypeSymbol, PluginClass> Lookup = new(SymbolEqualityComparer.Default);
        private ITypeSymbol BindPluginAttribute;

        public override void Init(GeneratorExecutionContext ctx) {
            base.Init(ctx);
            Bindings.Clear();
            Lookup.Clear();
            BindPluginAttribute = ctx.Compilation.GetTypeByMetadataName("SpacePi.Dashboard.API.BindPluginAttribute");
        }

        public override void Process(PluginClass plugin) {
            foreach (ISymbol sym in plugin.Symbol.GetMembers().Where(m => (m.Kind == SymbolKind.Field || m.Kind == SymbolKind.Property)
                && m.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, BindPluginAttribute))
                && m.DeclaredAccessibility == Accessibility.Public
                && !m.IsAbstract
                && !m.IsStatic
                && (m.Kind != SymbolKind.Property || !((IPropertySymbol) m).IsReadOnly)
                && (m.Kind != SymbolKind.Field || !((IFieldSymbol) m).IsReadOnly))) {
                BoundPlugin binding = new() {
                    Parent = plugin,
                    FieldName = sym.Name,
                    TargetClassSymbol = sym.Kind == SymbolKind.Property ? ((IPropertySymbol) sym).Type : ((IFieldSymbol) sym).Type
                };
                plugin.BoundPlugins.Add(binding);
                Bindings.Add(binding);
            }
            Lookup[plugin.Symbol] = plugin;
        }

        public override IEnumerable<PluginClass> Finish() {
            foreach (BoundPlugin binding in Bindings) {
                binding.TargetClass = Lookup[binding.TargetClassSymbol];
            }
            return base.Finish();
        }
    }
}
