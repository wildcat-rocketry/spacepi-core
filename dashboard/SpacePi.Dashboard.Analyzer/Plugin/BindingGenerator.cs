using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public class BindingGenerator {
        private readonly ITypeSymbol BindContextAttribute;
        private readonly ITypeSymbol BindPluginAttribute;
        private readonly ITypeSymbol IEnumerable_1;

        private IEnumerable<Diagnostic> GenerateRecursive<TTarget>(ITypeSymbol symbol, List<Binding<TTarget>> targetBindings, ITypeSymbol attribute) where TTarget : IEquatable<TTarget>, IClassBindingTarget, new() {
            foreach (ISymbol sym in symbol.GetMembers().Where(s => s.GetAttributes().Any(a => SymbolEqualityComparer.Default.Equals(a.AttributeClass, attribute)))) {
                if (sym is not IPropertySymbol prop) {
                    yield return Diagnostics.BindingMustBeProperty.Create(sym, sym.ContainingType, sym.Name);
                    continue;
                }
                if (prop.SetMethod?.DeclaredAccessibility != Accessibility.Public) {
                    yield return Diagnostics.BoundPropertyNeedsPublicSet.Create(prop, prop.ContainingType, prop.Name);
                    continue;
                }
                Binding<TTarget> binding = null;
                if (SymbolEqualityComparer.Default.Equals(IEnumerable_1, prop.Type.OriginalDefinition)) {
                    if (ArrayBinding<TTarget>.GetInterfaceType(prop) == null) {
                        yield return Diagnostics.ArrayBindingMustBeInterface.Create(prop, prop.ContainingType, prop.Name, prop.Type);
                    } else {
                        binding = new ArrayBinding<TTarget>();
                    }
                } else if (prop.Type.TypeKind == TypeKind.Interface) {
                    binding = new InterfaceBinding<TTarget>();
                } else {
                    binding = new ClassBinding<TTarget>();
                }
                if (binding != null) {
                    binding.Property = prop;
                    targetBindings.Add(binding);
                }
            }
            if (symbol.BaseType != null) {
                foreach (Diagnostic diag in GenerateRecursive(symbol.BaseType, targetBindings, attribute)) {
                    yield return diag;
                }
            }
        }

        private IEnumerable<Diagnostic> Generate<TTarget>(ITypeSymbol symbol, List<Binding<TTarget>> targetBindings, ITypeSymbol attribute, string attributeName) where TTarget : IEquatable<TTarget>, IClassBindingTarget, new() {
            if (attribute == null) {
                yield return Diagnostics.MissingAPIType.Create(attributeName);
                yield break;
            }
            if (IEnumerable_1 == null) {
                yield return Diagnostics.MissingAPIType.Create(Types.IEnumerable_1);
                yield break;
            }
            foreach (Diagnostic diag in GenerateRecursive(symbol, targetBindings, attribute)) {
                yield return diag;
            }
        }

        public IEnumerable<Diagnostic> GeneratePluginBindings(ITypeSymbol symbol, IPluginBindable bindable) => Generate(symbol, bindable.PluginBindings, BindPluginAttribute, Types.BindPluginAttribute);

        public IEnumerable<Diagnostic> GenerateContextBindings(ITypeSymbol symbol, IContextBindable bindable) => Generate(symbol, bindable.ContextBindings, BindContextAttribute, Types.BindContextAttribute);

        public BindingGenerator(Compilation compilation) {
            BindContextAttribute = compilation.GetTypeByMetadataName(Types.BindContextAttribute);
            BindPluginAttribute = compilation.GetTypeByMetadataName(Types.BindPluginAttribute);
            IEnumerable_1 = compilation.GetTypeByMetadataName(Types.IEnumerable_1);
        }
    }
}
