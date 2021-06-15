using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record PluginInstance {
        public PluginClass Parent;
        public string FQCN;
        public ITypeSymbol Symbol;
        public int Index;
        public List<BoundPlugin<PluginInstance, PluginClass>> BoundPlugins = new();
        public List<BoundPlugin<PluginInstance, PluginInstance>> BoundInstances = new();
    }
}
