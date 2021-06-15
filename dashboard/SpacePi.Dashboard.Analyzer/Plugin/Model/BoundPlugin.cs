using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record BoundPlugin<TSource, TDest> {
        public TSource Parent;
        public string FieldName;
        public ITypeSymbol TargetClassSymbol;
        public TDest TargetClass;
    }
}
