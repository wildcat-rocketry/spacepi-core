using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// An instance of a binding generated in code
    /// </summary>
    public record BindingInstance {
        /// <summary>
        /// The base binding
        /// </summary>
        public Binding Binding;
        /// <summary>
        /// The factory instance the binding is targetting
        /// </summary>
        public BindingFactoryInstance Factory;
        /// <summary>
        /// A list of object instances the binding is targetting
        /// </summary>
        public FactoryObjectInstance[] Objects;
        /// <summary>
        /// A dictionary of object instances the binding is targetting
        /// </summary>
        public IDictionaryHasher<FactoryObjectInstance> ObjectDict;

        /// <summary>
        /// Updates bindings to their instantized objects
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="instances">A map of factories to their instances for all factories accessible to this context</param>
        public void Rebind(Context ctx, Dictionary<BindingFactory, BindingFactoryInstance> instances) {
            if (!instances.TryGetValue(Binding.Factory, out Factory)) {
                ctx.Diagnostics.BindingNotValidInContext.Report(Binding.Symbol, Binding.Symbol);
                return;
            }
            Objects = Binding?.Objects?.Select(o => Factory.Objects[o])?.ToArray();
            ObjectDict = Binding.ObjectDict?.Rehash(Binding.ObjectDict?.ToDictionary(t => t.Key, t => Factory.Objects[t.Value]));
        }

        /// <summary>
        /// Creates a new instance of the binding
        /// </summary>
        /// <param name="binding">The base binding</param>
        public BindingInstance(Binding binding) => Binding = binding;
    }
}
