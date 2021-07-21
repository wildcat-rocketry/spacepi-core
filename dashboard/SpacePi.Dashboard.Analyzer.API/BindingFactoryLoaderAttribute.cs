using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a method to be called after all bindings have been completed
    /// </summary>
    [AttributeUsage(AttributeTargets.Method)]
    public class BindingFactoryLoaderAttribute : Attribute {
    }
}
