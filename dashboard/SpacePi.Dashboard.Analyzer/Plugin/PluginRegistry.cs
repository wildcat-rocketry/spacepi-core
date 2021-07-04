using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public class PluginRegistry {
        private readonly Dictionary<PluginClass, PluginClass> PluginSet = new();
        private readonly Dictionary<int, PluginClass> PriorityMap = new();
        private readonly Compilation Compilation;
        private readonly ITypeSymbol GeneratePluginFactoryAttribute;
        private readonly ITypeSymbol IPlugin;
        private readonly ITypeSymbol PluginAttribute;

        public IEnumerable<PluginClass> Plugins => PluginSet.Keys;

        public void Register(ref PluginClass plugin, out ITypeSymbol dupedSymbol) {
            if (PluginSet.TryGetValue(plugin, out PluginClass oldPlugin)) {
                plugin.CopyTo(oldPlugin);
                plugin = oldPlugin;
            } else {
                PluginSet.Add(plugin, plugin);
            }
            if (PriorityMap.TryGetValue(plugin.Priority, out oldPlugin) && plugin != oldPlugin) {
                dupedSymbol = oldPlugin.Symbol;
            } else {
                dupedSymbol = null;
            }
            PriorityMap[plugin.Priority] = plugin;
        }

        private IEnumerable<Diagnostic> TryParse(INamedTypeSymbol symbol, PluginClass[] plugin) {
            if (GeneratePluginFactoryAttribute == null) {
                yield return Diagnostics.MissingAPIType.Create(Types.GeneratePluginFactoryAttribute);
                yield break;
            }
            if (IPlugin == null) {
                yield return Diagnostics.MissingAPIType.Create(Types.IPlugin);
                yield break;
            }
            if (PluginAttribute == null) {
                yield return Diagnostics.MissingAPIType.Create(Types.PluginAttribute);
                yield break;
            }
            AttributeData attr = symbol.GetAttributes().FirstOrDefault(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, GeneratePluginFactoryAttribute));
            bool isClass = symbol.TypeKind == TypeKind.Class;
            bool isAbstract = symbol.IsAbstract;
            bool isGeneric = symbol.IsGenericType;
            if (attr != null) {
                if (!isClass) {
                    yield return Diagnostics.PluginNotClass.Create(symbol, symbol);
                }
                if (isAbstract) {
                    yield return Diagnostics.PluginIsAbstract.Create(symbol, symbol);
                }
                if (isGeneric) {
                    yield return Diagnostics.PluginIsGeneric.Create(symbol, symbol);
                }
                plugin[0] = new PluginFactoryClass {
                    Symbol = new PluginFactorySymbol(symbol, Compilation),
                    GenerateMain = attr.ConstructorArguments.Select(a => a.Value).Cast<bool>().FirstOrDefault()
                };
            } else {
                attr = symbol.GetAttributes().FirstOrDefault(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, PluginAttribute));
                bool hasIface = symbol.AllInterfaces.Contains(IPlugin, SymbolEqualityComparer.Default);
                bool hasCtor = symbol.InstanceConstructors.Any(c => c.Parameters.IsEmpty && c.DeclaredAccessibility == Accessibility.Public);
                if (attr == null) {
                    if (hasIface && isClass && !isAbstract && !isGeneric && hasCtor) {
                        yield return Diagnostics.PluginNotAttributed.Create(symbol, symbol);
                    }
                } else {
                    if (!hasIface) {
                        yield return Diagnostics.PluginMissingInterface.Create(symbol, symbol);
                    }
                    if (!isClass) {
                        yield return Diagnostics.PluginNotClass.Create(symbol, symbol);
                    }
                    if (isAbstract) {
                        yield return Diagnostics.PluginIsAbstract.Create(symbol, symbol);
                    }
                    if (isGeneric) {
                        yield return Diagnostics.PluginIsGeneric.Create(symbol, symbol);
                    }
                    if (!hasCtor) {
                        yield return Diagnostics.PluginHasBadConstructor.Create(symbol, symbol);
                    }
                    if (hasIface && isClass && !isAbstract && !isGeneric && hasCtor) {
                        int priority = attr.ConstructorArguments.Select(a => a.Value).Cast<int>().FirstOrDefault();
                        plugin[0] = new() {
                            Symbol = symbol,
                            Priority = priority
                        };
                    }
                }
            }
        }

        public IEnumerable<Diagnostic> TryParse(INamedTypeSymbol symbol, out PluginClass plugin) {
            PluginClass[] pluginPtr = new PluginClass[1];
            IEnumerable<Diagnostic> diags = TryParse(symbol, pluginPtr).ToList();
            plugin = pluginPtr[0];
            return diags;
        }

        public void GenerateIndices() => Plugins.OrderBy(p => p.Priority).Select((p, i) => p.Index = i).ToList();

        public PluginRegistry(Compilation compilation) {
            Compilation = compilation;
            GeneratePluginFactoryAttribute = Compilation.GetTypeByMetadataName(Types.GeneratePluginFactoryAttribute);
            IPlugin                        = Compilation.GetTypeByMetadataName(Types.IPlugin);
            PluginAttribute                = Compilation.GetTypeByMetadataName(Types.PluginAttribute);
        }
    }
}
