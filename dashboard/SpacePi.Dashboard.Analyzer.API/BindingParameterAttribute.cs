using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a boxing method parameter should be set to a user-defined parameter
    /// Also used to mark a parameter to an attribute constructor as a user-defined parameter
    /// </summary>
    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingParameterAttribute : Attribute {
        /// <summary>
        /// Attribute constructor
        /// </summary>
        /// <param name="name">The name of the user-defined parameter</param>
        public BindingParameterAttribute(string name) {
        }
    }
}
