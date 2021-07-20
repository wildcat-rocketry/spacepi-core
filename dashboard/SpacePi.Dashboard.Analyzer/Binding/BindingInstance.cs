using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record BindingInstance {
        public Binding Binding;
        public BindingFactoryInstance Factory;
        public FactoryObjectInstance[] Objects;
        public IDictionaryHasher<FactoryObjectInstance> ObjectDict;

        public void Rebind(Context ctx, Dictionary<BindingFactory, BindingFactoryInstance> instances) {
            if (!instances.TryGetValue(Binding.Factory, out Factory)) {
                ctx.Diagnostics.BindingNotValidInContext.Report(Binding.Symbol, Binding.Symbol);
                return;
            }
            Objects = Binding?.Objects?.Select(o => Factory.Objects[o])?.ToArray();
            ObjectDict = Binding.ObjectDict?.Rehash(Binding.ObjectDict?.ToDictionary(t => t.Key, t => Factory.Objects[t.Value]));
        }

        public BindingInstance(Binding binding) => Binding = binding;
    }
}
