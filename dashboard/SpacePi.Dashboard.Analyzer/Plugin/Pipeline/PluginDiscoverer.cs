using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class PluginDiscoverer : PipelineNode<Null, PluginClass> {
        private Stack<(string, INamespaceSymbol)> NamespaceStack;
        private ITypeSymbol PluginAttribute;
        private ITypeSymbol IPlugin;

        public override void Init(GeneratorExecutionContext ctx) {
            NamespaceStack = new();
            NamespaceStack.Push(("", ctx.Compilation.GlobalNamespace));
            PluginAttribute = ctx.Compilation.GetTypeByMetadataName("SpacePi.Dashboard.API.PluginAttribute");
            IPlugin = ctx.Compilation.GetTypeByMetadataName("SpacePi.Dashboard.API.IPlugin");
        }

        public override IEnumerable<PluginClass> ProcessMany(Null @in) {
            while (NamespaceStack.Count > 0) {
                (string fqnn, INamespaceSymbol symbol) = NamespaceStack.Pop();
                foreach (INamespaceSymbol sub in symbol.GetNamespaceMembers()) {
                    NamespaceStack.Push(($"{fqnn}{sub.Name}.", sub));
                }
                foreach (ITypeSymbol type in
                    symbol.GetTypeMembers().Where(type => type.TypeKind == TypeKind.Class
                        && type.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, PluginAttribute))
                        && type.AllInterfaces.Contains(IPlugin, SymbolEqualityComparer.Default)
                        && !type.IsAbstract)) {
                    yield return new PluginClass {
                        FQCN = $"{fqnn}{type.Name}",
                        Symbol = type
                    };
                }
            }
        }
    }
}
