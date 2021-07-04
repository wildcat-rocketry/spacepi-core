using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public record ClassBinding<TTarget> : Binding<TTarget>, IClassBinding where TTarget : IEquatable<TTarget>, IClassBindingTarget, new() {
        public TTarget Target { get; set; }

        IClassBindingTarget IClassBinding.Target => Target;

        public override IEnumerable<Diagnostic> TryBind(Dictionary<TTarget, TTarget> targets) {
            if (targets.TryGetValue(new() { Symbol = Property.Type }, out TTarget target)) {
                Target = target;
            } else {
                Target = default;
                yield return Diagnostics.ClassBindingNotFound.Create(Property, Property.ContainingType, Property.Name, Property.Type);
            }
        }
    }
}
