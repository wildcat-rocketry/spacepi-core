using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public record InterfaceBinding<TTarget> : Binding<TTarget>, IClassBinding where TTarget : IEquatable<TTarget>, IClassBindingTarget {
        public TTarget Target { get; set; }

        IClassBindingTarget IClassBinding.Target => Target;

        public override IEnumerable<Diagnostic> TryBind(Dictionary<TTarget, TTarget> targets) {
            TTarget[] impls = targets.Keys.Where(p => p.Symbol.AllInterfaces.Contains(Property.Type, SymbolEqualityComparer.Default)).Take(2).ToArray();
            switch (impls.Length) {
                case 0:
                    yield return Diagnostics.InterfaceBindingNotFound.Create(Property, Property.ContainingType, Property.Name, Property.Type);
                    break;
                case 1:
                    Target = impls[0];
                    break;
                default:
                    yield return Diagnostics.InterfaceBindingFound.Create(Property, Property.ContainingType, Property.Name, Property.Type);
                    break;
            }
        }
    }
}
