using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Diagnostics;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public class PluginDiagnosticAnalyzer : DiagnosticAnalyzer<PluginAnalyzer> {
        protected override PluginAnalyzer CreateAnalyzer(Compilation compilation) {
            PluginAnalyzer analyzer = new(compilation);
            analyzer.AnalyzeAll();
            return analyzer;
        }

        protected override void RegisterAnalyzer(DiagnosticRegistrar<PluginAnalyzer> registrar) => registrar.RegisterSymbolAction(RunAnalyzer, SymbolKind.NamedType);

        private IEnumerable<IEnumerable<Diagnostic>> RunAnalyzer(SymbolAnalysisContext context, PluginAnalyzer analyzer) {
            if (context.Symbol is INamedTypeSymbol sym) {
                yield return analyzer.AnalyzePlugin(sym, out PluginClass plugin);
                yield return analyzer.CheckProbableContext(sym);
                if (plugin != null) {
                    yield return analyzer.BindPlugin(plugin);
                }
            }
        }
    }
}
