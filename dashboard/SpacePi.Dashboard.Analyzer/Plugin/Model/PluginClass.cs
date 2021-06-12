using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record PluginClass {
        public string FQCN;
        public ITypeSymbol Symbol;
        public int Index;
    }
}
