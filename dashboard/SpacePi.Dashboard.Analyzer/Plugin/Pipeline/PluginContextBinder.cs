using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PluginContextBinder : PluginBinder<ContextClass, ContextClass> {
        public override PluginClass GetPlugin(ContextClass source) => source.Parent;

        public override void RegisterBinding(ContextClass source, BoundPlugin<ContextClass, PluginClass> binding) => source.BoundPlugins.Add(binding);

        public override void Process(ContextClass context) => ProcessSource(context, context.Symbol);
    }
}
