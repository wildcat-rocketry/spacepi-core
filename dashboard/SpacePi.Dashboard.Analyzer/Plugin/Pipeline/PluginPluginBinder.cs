using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class PluginPluginBinder : PluginBinder<PluginClass> {
        public override Diagnostic GenerateMissingDiagnostic(BoundPlugin<PluginClass, PluginClass> binding) =>
            Diagnostic.Create(Diagnostics.UnknownBindingClass, null, binding.TargetClassSymbol, binding.Parent.Symbol, binding.FieldName);

        public override void RegisterBinding(PluginClass source, BoundPlugin<PluginClass, PluginClass> binding) => source.BoundPlugins.Add(binding);

        public override void Process(PluginClass plugin) {
            ProcessSource(plugin, plugin.Symbol);
            RegisterSource(plugin, plugin.Symbol);
        }
    }
}
