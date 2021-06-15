using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record ContextClass {
        public PluginClass Parent;
        public string FQCN;
        public ITypeSymbol Symbol;
        public int Index;
        public List<BoundPlugin<ContextClass, PluginClass>> BoundPlugins = new();
        public List<BoundPlugin<ContextClass, ContextClass>> BoundContexts = new();
    }
}
