using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// Marks a binding attribute constructor parameter as the name of a parameter which is used to filter instances
    /// included in the binding.  Any included items must have the ID of the current object in this property on their
    /// declarative attribute.
    /// </summary>
    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingMatchIDAttribute : Attribute {
    }
}
