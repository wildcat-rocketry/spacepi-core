using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record FactoryObjectInstance {
        public FactoryObject FactoryObject;
        public BindingInstance[] Bindings;

        public FactoryObjectInstance(FactoryObject obj) {
            FactoryObject = obj;
            Bindings = obj.Bindings.Select(o => new BindingInstance(o)).ToArray();
        }
    }
}
