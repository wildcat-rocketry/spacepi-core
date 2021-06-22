using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class PluginContextBinder : PluginBinder<ContextClass> {
        public override Diagnostic GenerateMissingDiagnostic(BoundPlugin<ContextClass, PluginClass> binding) =>
            Diagnostic.Create(Diagnostics.UnknownBindingClass, null, binding.TargetClassSymbol, binding.Parent.Symbol, binding.FieldName);

        public override void RegisterBinding(ContextClass source, BoundPlugin<ContextClass, PluginClass> binding) => source.BoundPlugins.Add(binding);

        public override void Process(ContextClass context) {
            ProcessSource(context, context.Symbol);
            RegisterSource(context.Parent, context.Parent.Symbol);
        }
    }
}
