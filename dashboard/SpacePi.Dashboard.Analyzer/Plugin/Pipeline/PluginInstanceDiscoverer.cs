using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PluginInstanceDiscoverer : PluginPipeline {
        private ITypeSymbol IPlugin;

        public override void Init(GeneratorExecutionContext ctx) => IPlugin = ctx.Compilation.GetTypeByMetadataName("SpacePi.Dashboard.API.IPlugin`1");

        public override void Process(PluginClass plugin) {
            plugin.Instance = plugin.Symbol.AllInterfaces
                .Where(i => SymbolEqualityComparer.Default.Equals(i.OriginalDefinition, IPlugin))
                .Select(i => i.TypeArguments.First())
                .Select(c => new PluginInstance {
                    Parent = plugin,
                    FQCN = c.ToDisplayString(SymbolDisplayFormat.FullyQualifiedFormat),
                    Symbol = c
                }).ToArray();
        }
    }
}
