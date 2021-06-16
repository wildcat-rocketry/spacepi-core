using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class PluginSorter : PipelineNode<PluginClass, PluginClass> {
        private readonly SortedList<int, List<PluginClass>> Sorter = new();
        private ITypeSymbol PluginAttribute;

        public override void Init(GeneratorExecutionContext ctx) {
            Sorter.Clear();
            PluginAttribute = ctx.Compilation.GetTypeByMetadataName("SpacePi.Dashboard.API.PluginAttribute");
        }

        public override IEnumerable<PluginClass> ProcessMany(PluginClass plugin) {
            int priority = plugin.Symbol.GetAttributes()
                .First(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, PluginAttribute))
                .ConstructorArguments.Select(a => a.Value)
                .Cast<int>().FirstOrDefault();
            if (Sorter.TryGetValue(priority, out List<PluginClass> list)) {
                list.Add(plugin);
            } else {
                list = new();
                list.Add(plugin);
                Sorter.Add(priority, list);
            }
            return Enumerable.Empty<PluginClass>();
        }

        public override IEnumerable<PluginClass> Finish() {
            return Sorter.SelectMany(o => o.Value);
        }
    }
}
