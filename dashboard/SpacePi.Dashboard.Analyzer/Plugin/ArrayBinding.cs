using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public record ArrayBinding<TTarget> : Binding<TTarget>, IArrayBinding where TTarget : IEquatable<TTarget>, IClassBindingTarget {
        public IEnumerable<TTarget> Targets { get; set; }

        IEnumerable<IClassBindingTarget> IArrayBinding.Targets => Targets.Cast<IClassBindingTarget>();

        public static ITypeSymbol GetInterfaceType(IPropertySymbol prop) {
            if (prop.Type is not INamedTypeSymbol symbol) {
                return null;
            }
            ITypeSymbol t = symbol.TypeArguments.FirstOrDefault();
            if (t == null) {
                return null;
            }
            if (t.TypeKind != TypeKind.Interface) {
                return null;
            }
            return t;
        }

        public override IEnumerable<Diagnostic> TryBind(Dictionary<TTarget, TTarget> targets) {
            ITypeSymbol ifaceType = GetInterfaceType(Property);
            Targets = targets.Keys.Where(p => p.Symbol.AllInterfaces.Contains(ifaceType, SymbolEqualityComparer.Default)).ToArray();
            yield break;
        }
    }
}
