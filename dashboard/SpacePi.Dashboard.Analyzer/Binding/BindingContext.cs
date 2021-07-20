using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public class BindingContext {
        public readonly BindingFactory[] Factories;
        public readonly BindingFactoryInstance[] Roots;
        public readonly GeneratedClass MainClass = new();

        private IEnumerable<BindingFactoryInstance> AllFactoriesRecursive(IEnumerable<BindingFactoryInstance> roots) =>
            roots.Concat(roots.SelectMany(r => AllFactoriesRecursive(r.Subfactories)));

        public IEnumerable<BindingFactoryInstance> AllFactories => AllFactoriesRecursive(Roots);

        public BindingContext(Context ctx) {
            Factories = BindingFactory.ParseAll(ctx).ToArray();
            foreach (BindingFactory factory in Factories) {
                factory.FindObjects(ctx);
            }
            foreach (FactoryObject obj in Factories.SelectMany(f => f.Objects)) {
                obj.Bindings = Factories.SelectMany(f => Binding.ParseAll(ctx, f, obj.ObjectType)).ToArray();
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
