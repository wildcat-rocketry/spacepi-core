using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a boxing method parameter should be set to the constructed object
    /// </summary>
    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingFactoryObjectAttribute : Attribute {
    }
}
