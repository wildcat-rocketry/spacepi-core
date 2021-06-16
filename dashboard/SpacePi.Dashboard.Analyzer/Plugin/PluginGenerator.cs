using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;
using SpacePi.Dashboard.Analyzer.Plugin.Pipeline;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    [Generator]
    public class PluginGenerator : DebugSourceGenerator {
        public override void DebugExecute(GeneratorExecutionContext context) {
            ITypeSymbol GeneratePluginFactoryAttribute = context.Compilation.GetTypeByMetadataName(Types.GeneratePluginFactoryAttribute);
            if (context.Compilation.Assembly.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, GeneratePluginFactoryAttribute))) {
                PipelineGenerator.RunSourcePipeline(context,
                    new PluginDiscoverer() +
                    new PluginSorter() +
                    new PluginIndexer() +
                    new PluginPluginBinder() +
                    new ContextDiscoverer() +
                    new ContextBranch(
                        new ContextIndexer() +
                        new PluginContextBinder() +
                        new ContextContextBinder()
                    ) +
                    (new PluginFactoryGenerator() | new PluginFactoryBindingGenerator() | new ContextFactoryGenerator() | new ContextFactoryBindingGenerator())
                );
            }
        }
    }
}
