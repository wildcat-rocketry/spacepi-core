using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record Binding {
        public enum Modes {
            Single,
            List,
            Factory
        }

        public IPropertySymbol Symbol;
        public string Id;
        public Modes Mode;
        public BindingFactory Factory;
        public FactoryObject[] Objects;

        public static IEnumerable<Binding> ParseAll(Context ctx, BindingFactory factory, INamedTypeSymbol cls) {
            Binding obj = null;
            foreach ((IPropertySymbol prop, AttributeData attr) in cls.GetAllMembers()
                .OfType<IPropertySymbol>()
                .BeginValidation()
                .HasAttribute(factory.BindingAttribute, a => a)
                .WhereValid()) {
                if (obj == null) {
                    obj = new();
                }
                if (!prop.BeginValidation()
                    .HasPublicSetter<IPropertySymbol>()
                    .Require(ctx.Diagnostics)) {
                    continue;
                }
                obj.Symbol = prop;
                obj.Id = attr.ConstructorArguments.Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))
                    .Where(t => t.b.BeginValidation().HasAttribute(ctx.BindingIDAttribute).Check())
                    .Select(t => t.a.Value.ToString())
                    .FirstOrDefault();
                if (prop.Type.DEquals(factory.Symbol)) {
                    obj.Mode = Modes.Factory;
                } else if (prop.Type.OriginalDefinition.DEquals(ctx.IEnumerable_1)) {
                    obj.Mode = Modes.List;
                } else {
                    obj.Mode = Modes.Single;
                }
                obj.Factory = factory;
                yield return obj;
                obj = null;
            }
        }

        public static void PerformBinding(Context ctx, IEnumerable<Binding> bindings) {
            foreach (Binding binding in bindings) {
                binding.Objects = null;
                if (binding.Mode == Modes.Factory) {
                    // Nothing to bind to explicitly
                    continue;
                }
                IEnumerable<FactoryObject> filter = binding.Factory.Objects
                    .Where(o => ((ITypeSymbol) o.ObjectType).BeginValidation()
                        .IsCastableTo(binding.Symbol.Type)
                        .Check());
                if (binding.Id != null) {
                    filter = filter.Where(o => o.Id == binding.Id);
                }
                if (binding.Mode == Modes.Single) {
                    filter = filter.Take(2);
                }
                binding.Objects = filter.ToArray();
                if (binding.Mode == Modes.Single && binding.Objects.Length != 1) {
                    ctx.Diagnostics.ExactlyOneBindingTargetNotFound.Report(binding.Symbol, binding.Objects.Length, binding.Symbol);
                }
            }
        }
    }
}
