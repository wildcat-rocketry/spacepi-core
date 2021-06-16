using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class ContextBranch : BufferedPipeline<PluginClass> {
        private readonly GenericPipeline ContextPipeline;

        public override void Init(GeneratorExecutionContext ctx) => ContextPipeline.Init(ctx);

        public override void Process(PluginClass plugin) {
            foreach (ContextClass ctx in plugin.Contexts) {
                ContextPipeline.ProcessGeneric(ctx).ToArray();
            }
        }

        public override IEnumerable<PluginClass> Finish() {
            ContextPipeline.FinishGeneric().ToArray();
            return base.Finish();
        }

        public ContextBranch(GenericPipeline contextPipeline) => ContextPipeline = contextPipeline;
    }
}
