using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public class BindingContext {
        public readonly BindingFactory[] Factories;

        public BindingContext(Context ctx) {
            Factories = BindingFactory.ParseAll(ctx).ToArray();
            foreach (BindingFactory factory in Factories) {
                factory.FindObjects(ctx);
            }
            foreach (FactoryObject obj in Factories.SelectMany(f => f.Objects)) {
                obj.Bindings = Factories.SelectMany(f => Binding.ParseAll(ctx, f, obj.ObjectType)).ToArray();
            }
            Binding.PerformBinding(ctx, Factories.SelectMany(f => f.Objects).SelectMany(o => o.Bindings));
        }
    }
}
