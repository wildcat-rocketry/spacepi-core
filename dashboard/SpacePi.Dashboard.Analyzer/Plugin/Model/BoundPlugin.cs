using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    record BoundPlugin<TSource, TDest> : DebuggableRecord {
        public TSource Parent;
        public string FieldName;
        public ITypeSymbol TargetClassSymbol;
        public TDest TargetClass;

        protected override bool PrintMembers(StringBuilder str) {
            str.Append($"\"{nameof(Parent)}\": \"(recursion)\", ");
            str.Append($"\"{nameof(FieldName)}\": \"{FieldName}\", ");
            str.Append($"\"{nameof(TargetClassSymbol)}\": \"{TargetClassSymbol}\", ");
            str.Append($"\"{nameof(TargetClass)}\": \"(recursion)\", ");
            return true;
        }
    }
}
