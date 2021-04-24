﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;

namespace FFAero.SpacePi.Dashboard.Analyzer {
    [Generator]
    public class PluginGenerator : ISourceGenerator {
        public void Execute(GeneratorExecutionContext context) {
            StringBuilder str = new(@"
using System;
using FFAero.SpacePi.Dashboard.API;

namespace FFAero.SpacePi.Dashboard.Analyzer {
    public static class PluginContext {
        public static void Setup() {
            PluginFactory.Initialize(new IPlugin[] {");
            Stack<(string, INamespaceSymbol)> namespaceStack = new();
            namespaceStack.Push(("", context.Compilation.GlobalNamespace));
            ITypeSymbol PluginAttribute = context.Compilation.GetTypeByMetadataName("FFAero.SpacePi.Dashboard.API.PluginAttribute");
            ITypeSymbol IPlugin = context.Compilation.GetTypeByMetadataName("FFAero.SpacePi.Dashboard.API.IPlugin");
            while (namespaceStack.Count > 0) {
                (string, INamespaceSymbol) ns = namespaceStack.Pop();
                foreach (INamespaceSymbol sub in ns.Item2.GetNamespaceMembers()) {
                    namespaceStack.Push(($"{ns.Item1}{sub.Name}.", sub));
                }
                foreach (ITypeSymbol type in
                    ns.Item2.GetTypeMembers().Where(type => type.TypeKind == TypeKind.Class
                        && type.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, PluginAttribute))
                        && type.AllInterfaces.Contains(IPlugin, SymbolEqualityComparer.Default)
                        && !type.IsAbstract)) {
                    str.Append($@"
                new {ns.Item1}{type.Name}(),");
                }
            }
            str.Append(@"
            });
        }
    }
}
");
            context.AddSource("PluginContext", SourceText.From(str.ToString(), Encoding.UTF8));
        }

        public void Initialize(GeneratorInitializationContext context) {
        }
    }
}
