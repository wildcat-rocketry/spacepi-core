using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record PluginClass : DebuggableRecord {
        public string FQCN;
        public ITypeSymbol Symbol;
        public int Index;
        public ContextClass[] Contexts = Array.Empty<ContextClass>();
        public List<BoundPlugin<PluginClass, PluginClass>> BoundPlugins = new();

        protected override bool PrintMembers(StringBuilder str) {
            str.Append($"\"{nameof(FQCN)}\": \"{FQCN}\", ");
            str.Append($"\"{nameof(Symbol)}\": \"{Symbol}\", ");
            str.Append($"\"{nameof(Index)}\": {Index}, ");
            str.Append($"\"{nameof(Contexts)}\": ");
            PrintList(str, Contexts);
            str.Append(", ");
            str.Append($"\"{nameof(BoundPlugins)}\": ");
            return PrintList(str, BoundPlugins);
        }
    }
}
