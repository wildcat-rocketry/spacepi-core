using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    [AttributeUsage(AttributeTargets.Class)]
    public class BindingFactoryAttribute : Attribute {
        public BindingFactoryAttribute(Type declarativeAttribute, Type bindingAttribute) {
        }
    }
}
