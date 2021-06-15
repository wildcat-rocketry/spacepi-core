using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PluginPluginBinder : PluginBinder<PluginClass, PluginClass> {
        public override PluginClass GetPlugin(PluginClass source) => source;

        public override void RegisterBinding(PluginClass source, BoundPlugin<PluginClass, PluginClass> binding) => source.BoundPlugins.Add(binding);

        public override void Process(PluginClass plugin) => ProcessSource(plugin, plugin.Symbol);
    }
}
