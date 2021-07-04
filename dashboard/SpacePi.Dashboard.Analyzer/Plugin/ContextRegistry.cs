using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public class ContextRegistry {
        private readonly Dictionary<ContextClass, ContextClass> ContextSet = new();
        private readonly ITypeSymbol IContext;
        private readonly ITypeSymbol IPlugin_1;

        public IEnumerable<ContextClass> Contexts => ContextSet.Keys;

        public void Register(ContextClass context, out PluginClass dupedPlugin) {
            dupedPlugin = null;
            if (ContextSet.TryGetValue(context, out ContextClass oldContext)) {
                if (oldContext.OwnerPlugin != oldContext.OwnerPlugin) {
                    dupedPlugin = oldContext.OwnerPlugin;
                }
                context.CopyTo(oldContext);
            } else {
                ContextSet.Add(context, context);
            }
        }

        public IEnumerable<Diagnostic> TryRegisterAll(PluginClass plugin) {
            if (IContext == null) {
                yield return Diagnostics.MissingAPIType.Create(Types.IContext);
                yield break;
            }
            if (IPlugin_1 == null) {
                yield return Diagnostics.MissingAPIType.Create(Types.IPlugin_1);
                yield break;
            }
            HashSet<ContextClass> removed = new(plugin.Contexts.Keys);
            foreach (INamedTypeSymbol factory in plugin.Symbol.AllInterfaces.Where(i => SymbolEqualityComparer.Default.Equals(IPlugin_1, i.OriginalDefinition))) {
                ITypeSymbol symbol = factory.TypeArguments.First();
                bool isValid;
                foreach (Diagnostic diag in CheckProbableContext(symbol, out isValid)) {
                    yield return diag;
                }
                if (isValid) {
                    ContextClass context = new() {
                        Symbol = symbol,
                        OwnerPlugin = plugin,
                        PluginInterface = factory
                    };
                    if (plugin.Contexts.TryGetValue(context, out ContextClass oldContext)) {
                        removed.Remove(context);
                        context.CopyTo(oldContext);
                        context = oldContext;
                    } else {
                        plugin.Contexts.Add(context, context);
                    }
                    Register(context, out PluginClass dupedPlugin);
                    if (dupedPlugin != null) {
                        yield return Diagnostics.ContextDuplicated.Create(symbol, symbol, dupedPlugin.Symbol, plugin.Symbol);
                        yield return Diagnostics.ContextDuplicated.Create(symbol, symbol, plugin.Symbol, dupedPlugin.Symbol);
                    }
                }
            }
            foreach (ContextClass toRemove in removed) {
                plugin.Contexts.Remove(toRemove);
            }
        }

        private IEnumerable<Diagnostic> CheckProbableContext(ITypeSymbol symbol, bool[] isValid) {
            bool isClass = symbol.TypeKind == TypeKind.Class;
            bool hasIface = symbol.AllInterfaces.Contains(IContext, SymbolEqualityComparer.Default);
            if (!isClass) {
                yield return Diagnostics.ContextNotClass.Create(symbol, symbol);
            }
            if (!hasIface) {
                yield return Diagnostics.ContextMissingInterface.Create(symbol, symbol);
            }
            isValid[0] = isClass && hasIface;
        }

        public IEnumerable<Diagnostic> CheckProbableContext(ITypeSymbol symbol, out bool isValid) {
            bool[] isValidPtr = new bool[1];
            IEnumerable<Diagnostic> diags = CheckProbableContext(symbol, isValidPtr).ToList();
            isValid = isValidPtr[0];
            return diags;
        }

        public void GenerateIndices() => Contexts.OrderBy(c => c.OwnerPlugin.Index).Select((c, i) => c.Index = i).ToList();

        public ContextRegistry(Compilation compilation) {
            IContext = compilation.GetTypeByMetadataName(Types.IContext);
            IPlugin_1 = compilation.GetTypeByMetadataName(Types.IPlugin_1);
        }
    }
}
