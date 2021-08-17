using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// A context containing all of the binding model
    /// </summary>
    public class BindingContext {
        /// <summary>
        /// A list of all factories defined in the source
        /// </summary>
        public readonly BindingFactory[] Factories;
        /// <summary>
        /// A list of all root factories declared in the factory tree
        /// </summary>
        public readonly BindingFactoryInstance[] Roots;
        /// <summary>
        /// The symbol to make into the program's main class
        /// </summary>
        public readonly GeneratedClass MainClass = new();

        /// <summary>
        /// Recursively generates a list of all factories within a set of root factories
        /// </summary>
        /// <param name="roots">The root factories</param>
        /// <returns>All root and decendent factories</returns>
        private IEnumerable<BindingFactoryInstance> AllFactoriesRecursive(IEnumerable<BindingFactoryInstance> roots) =>
            roots.Concat(roots.SelectMany(r => AllFactoriesRecursive(r.Subfactories)));

        /// <summary>
        /// A list of all factories declared anywhere in the tree
        /// </summary>
        public IEnumerable<BindingFactoryInstance> AllFactories => AllFactoriesRecursive(Roots);

        /// <summary>
        /// Parses the source context to generate the binding model
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        public BindingContext(Context ctx) {
            Factories = BindingFactory.ParseAll(ctx).ToArray();
            foreach (BindingFactory factory in Factories) {
                factory.FindObjects(ctx);
            }
            foreach (FactoryObject obj in Factories.SelectMany(f => f.Objects)) {
                obj.Bindings = Factories.SelectMany(f => Binding.ParseAll(ctx, f, obj)).ToArray();
            }
            Binding.PerformBinding(ctx, Factories.SelectMany(f => f.Objects).SelectMany(o => o.Bindings));
            Dictionary<ISymbol, BindingFactory> factoryLookup = Factories.ToDictionary(f => f.Symbol, f => f, SymbolEqualityComparer.Default);
            Roots = Factories.Where(f => f.GenerateEntryPoint).Select(f => new BindingFactoryInstance(ctx, f, factoryLookup, Array.Empty<BindingFactoryInstance>())).ToArray();
            foreach (BindingFactoryInstance f in AllFactories) {
                f.RebindAll(ctx);
            }
        }
    }
}
