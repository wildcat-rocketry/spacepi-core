using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record BindingFactoryInstance {
        public readonly GeneratedClass GeneratedClass = new();
        public BindingFactory BindingFactory;
        public BindingFactoryInstance[] Subfactories;
        public BindingFactoryInstance[] Parents;
        public Dictionary<FactoryObject, FactoryObjectInstance> Objects;
        public Dictionary<BindingFactory, BindingFactoryInstance> InContext;

        public void RebindAll(Context ctx) {
            foreach (BindingInstance binding in Objects.Values.SelectMany(o => o.Bindings)) {
                binding.Rebind(ctx, InContext);
            }
        }

        public BindingFactoryInstance(Context ctx, BindingFactory factory, Dictionary<ISymbol, BindingFactory> all, BindingFactoryInstance[] parents) {
            BindingFactoryInstance[] family = parents.Concat(new[] { this }).ToArray();
            BindingFactory = factory;
            Subfactories = factory.Subfactories.Select(p => {
                if (!all.TryGetValue(((INamedTypeSymbol) p.Type).TypeArguments[0], out BindingFactory f)) {
                    ctx.Diagnostics.UnknownBindingFactory.Report(p, p.Type);
                    return null;
                }
                return new BindingFactoryInstance(ctx, f, all, family);
            }).ToArray();
            Parents = parents;
            Objects = factory.Objects.ToDictionary(o => o, o => new FactoryObjectInstance(o));
            InContext = family.ToDictionary(o => o.BindingFactory, o => o);
        }
    }
}
