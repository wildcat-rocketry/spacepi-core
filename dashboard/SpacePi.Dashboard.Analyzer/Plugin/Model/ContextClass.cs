using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    public record ContextClass : DebuggableRecord {
        public PluginClass Parent;
        public string FQCN;
        public ITypeSymbol Symbol;
        public int Index;
        public List<BoundPlugin<ContextClass, PluginClass>> BoundPlugins = new();
        public List<BoundPlugin<ContextClass, ContextClass>> BoundContexts = new();

        protected override bool PrintMembers(StringBuilder str) {
            str.Append($"\"{nameof(Parent)}\": \"{Parent.FQCN}\", ");
            str.Append($"\"{nameof(FQCN)}\": \"{FQCN}\", ");
            str.Append($"\"{nameof(Symbol)}\": \"{Symbol}\", ");
            str.Append($"\"{nameof(Index)}\": {Index}, ");
            str.Append($"\"{nameof(BoundPlugins)}\": ");
            PrintList(str, BoundPlugins);
            str.Append(", ");
            str.Append($"\"{nameof(BoundContexts)}\": ");
            return PrintList(str, BoundContexts);
        }
    }
}
