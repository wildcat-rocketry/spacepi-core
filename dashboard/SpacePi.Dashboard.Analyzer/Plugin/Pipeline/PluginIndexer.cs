using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class PluginIndexer : SingularPipeline<PluginClass> {
        private int NextIndex;

        public override void Init(GeneratorExecutionContext ctx) => NextIndex = 0;

        public override void Process(PluginClass plugin) => plugin.Index = NextIndex++;
    }
}
