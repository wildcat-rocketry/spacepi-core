using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// An instance of a factory generated as a class
    /// </summary>
    public record BindingFactoryInstance {
        /// <summary>
        /// The class to generate for the factory
        /// </summary>
        public readonly GeneratedClass GeneratedClass = new();
        /// <summary>
        /// The base factory
        /// </summary>
        public BindingFactory BindingFactory;
        /// <summary>
        /// A list of factories dependent on this factory
        /// </summary>
        public BindingFactoryInstance[] Subfactories;
        /// <summary>
        /// A list of factories on which this factory depends
        /// </summary>
        public BindingFactoryInstance[] Parents;
        /// <summary>
        /// A list of all objects created by the factory, mapped to their instances
        /// </summary>
        public Dictionary<FactoryObject, FactoryObjectInstance> Objects;
        /// <summary>
        /// A map of factories to their instances for all factories accessible to this context
        /// </summary>
        public Dictionary<BindingFactory, BindingFactoryInstance> InContext;

        /// <summary>
        /// Updates bindings to their instantized objects
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        public void RebindAll(Context ctx) {
            foreach (BindingInstance binding in Objects.Values.SelectMany(o => o.Bindings)) {
                binding.Rebind(ctx, InContext);
            }
        }

        /// <summary>
        /// Creates a new instance of the factory
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="factory">The base factory</param>
        /// <param name="all">All factory instances in the compilation context</param>
        /// <param name="parents">The factories on which this factory depends</param>
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
