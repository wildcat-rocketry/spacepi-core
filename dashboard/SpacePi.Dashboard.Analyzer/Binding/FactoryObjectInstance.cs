using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// An instance of an object generated in code
    /// </summary>
    public record FactoryObjectInstance {
        /// <summary>
        /// The base object
        /// </summary>
        public FactoryObject FactoryObject;
        /// <summary>
        /// A list of binding instances within the instance
        /// </summary>
        public BindingInstance[] Bindings;

        /// <summary>
        /// Creates a new instance of the object
        /// </summary>
        /// <param name="obj">The base object</param>
        public FactoryObjectInstance(FactoryObject obj) {
            FactoryObject = obj;
            Bindings = obj.Bindings.Select(o => new BindingInstance(o)).ToArray();
        }
    }
}
