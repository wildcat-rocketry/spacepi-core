using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    [AttributeUsage(AttributeTargets.Property)]
    public class BindingFactoryArrayAttribute : Attribute {
        public BindingFactoryArrayAttribute(Type elementType) {
        }
    }
}
