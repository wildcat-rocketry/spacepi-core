using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public abstract record Binding<TTarget> : IBinding where TTarget : IEquatable<TTarget> {
        public IPropertySymbol Property { get; set; }

        public abstract IEnumerable<Diagnostic> TryBind(Dictionary<TTarget, TTarget> targets);
    }
}
