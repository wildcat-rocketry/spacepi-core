using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Pipeline;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    [Generator]
    public class PluginGenerator : DebugSourceGenerator {
        public override void DebugExecute(GeneratorExecutionContext context) {
            PipelineGenerator.RunSourcePipeline(context,
                new PluginDiscoverer() +
                new PluginIndexer() +
                (new PluginFactoryGenerator() | new ContextGenerator())
            );
        }
    }
}
