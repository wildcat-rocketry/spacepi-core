using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record PluginClass {
        public string FQCN;
        public ITypeSymbol Symbol;
        public int Index;
        public ContextClass[] Instance = Array.Empty<ContextClass>();
        public List<BoundPlugin<PluginClass, PluginClass>> BoundPlugins = new();
    }
}
