using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a boxing method parameter should be set to the priority
    /// Also used to mark a parameter to an attribute constructor as the priority
    /// </summary>
    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingPriorityAttribute : Attribute {
    }
}
