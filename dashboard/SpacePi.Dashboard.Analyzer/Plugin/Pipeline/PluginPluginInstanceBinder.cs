using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PluginPluginInstanceBinder : PluginBinder<PluginInstance> {
        public override PluginClass GetPlugin(PluginInstance source) => source.Parent;

        public override void RegisterBinding(PluginInstance source, BoundPlugin<PluginInstance, PluginClass> binding) => source.BoundPlugins.Add(binding);

        public override void Process(PluginClass plugin) {
            foreach (PluginInstance instance in plugin.Instance) {
                ProcessSource(instance, instance.Symbol);
            }
        }
    }
}
