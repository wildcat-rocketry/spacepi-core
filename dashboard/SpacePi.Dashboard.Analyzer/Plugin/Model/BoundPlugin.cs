using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record BoundPlugin {
        public PluginClass Parent;
        public string FieldName;
        public ITypeSymbol TargetClassSymbol;
        public PluginClass TargetClass;
    }
}
