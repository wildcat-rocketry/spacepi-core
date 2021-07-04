using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public class PluginAnalyzer : Analyzer {
        public readonly PluginRegistry PluginRegistry;
        public readonly ContextRegistry ContextRegistry;
        public readonly BindingGenerator BindingGenerator;
        public readonly Binder<PluginClass> PluginBinder = new();
        public readonly Binder<ContextClass> ContextBinder = new();

        private IEnumerable<INamedTypeSymbol> AllTypes {
            get {
                Stack<INamespaceSymbol> stack = new();
                stack.Push(Compilation.GlobalNamespace);
                while (stack.Count > 0) {
                    INamespaceSymbol ns = stack.Pop();
                    foreach (INamespaceSymbol sub in ns.GetNamespaceMembers()) {
                        stack.Push(sub);
                    }
                    foreach (INamedTypeSymbol type in ns.GetTypeMembers()) {
                        yield return type;
                    }
                }
            }
        }

        public IEnumerable<Diagnostic> AnalyzePlugin(INamedTypeSymbol symbol, PluginClass[] plugin) => RunWithDiagnostics(() => {
            foreach (Diagnostic diag in PluginRegistry.TryParse(symbol, out plugin[0])) {
                ReportDiagnostic(diag);
            }
            if (plugin[0] != null) {
                foreach (Diagnostic diag in BindingGenerator.GeneratePluginBindings(symbol, plugin[0])) {
                    plugin[0] = null;
                    ReportDiagnostic(diag);
                }
                if (plugin[0] != null) {
                    PluginRegistry.Register(ref plugin[0], out ITypeSymbol dupedSymbol);
                    foreach (Diagnostic diag in ContextRegistry.TryRegisterAll(plugin[0])) {
                        ReportDiagnostic(diag);
                    }
                    foreach (ContextClass context in plugin[0].Contexts.Keys) {
                        foreach (Diagnostic diag in BindingGenerator.GeneratePluginBindings(context.Symbol, context)
                            .Concat(BindingGenerator.GenerateContextBindings(context.Symbol, context))) {
                            plugin[0] = null;
                            ReportDiagnostic(diag);
                        }
                    }
                    if (plugin[0] != null) {
                        PluginBinder.AddTarget(plugin[0]);
                        foreach (ContextClass context in plugin[0].Contexts.Keys) {
                            ContextBinder.AddTarget(context);
                        }
                        if (dupedSymbol != null) {
                            ReportDiagnostic(Diagnostics.PluginHasDuplicatePriority.Create(symbol, symbol, dupedSymbol, plugin[0].Priority));
                            ReportDiagnostic(Diagnostics.PluginHasDuplicatePriority.Create(dupedSymbol, dupedSymbol, symbol, plugin[0].Priority));
                        }
                    }
                }
            }
        });

        public IEnumerable<Diagnostic> AnalyzePlugin(INamedTypeSymbol symbol, out PluginClass plugin) {
            PluginClass[] pluginPtr = new PluginClass[1];
            IEnumerable<Diagnostic> diags = AnalyzePlugin(symbol, pluginPtr).ToList();
            plugin = pluginPtr[0];
            return diags;
        }

        public IEnumerable<Diagnostic> BindPlugin(PluginClass plugin) => PluginBinder.Bind(plugin.PluginBindings);

        public IEnumerable<Diagnostic> BindContext(ContextClass context) =>
            PluginBinder.Bind(context.PluginBindings)
            .Concat(ContextBinder.Bind(context.ContextBindings));

        public IEnumerable<Diagnostic> BindAll() =>
            PluginRegistry.Plugins.SelectMany(p => BindPlugin(p))
            .Concat(ContextRegistry.Contexts.SelectMany(c => BindContext(c)));

        public IEnumerable<Diagnostic> AnalyzeAll() => AllTypes.SelectMany(t => AnalyzePlugin(t, out PluginClass _));

        public IEnumerable<Diagnostic> CheckProbableContext(INamedTypeSymbol symbol) {
            bool hasError = ContextRegistry.CheckProbableContext(symbol, out bool isValid).Any();
            if (!isValid && !hasError && !symbol.IsAbstract && !symbol.IsGenericType) {
                yield return Diagnostics.ContextNotRegistered.Create(symbol, symbol);
            }
        }

        public IEnumerable<Diagnostic> FindAllProbableContexts() => AllTypes.SelectMany(t => CheckProbableContext(t));

        public PluginAnalyzer(Compilation compilation) : base(compilation) {
            PluginRegistry = new(Compilation);
            ContextRegistry = new(Compilation);
            BindingGenerator = new(Compilation);
        }
    }
}
