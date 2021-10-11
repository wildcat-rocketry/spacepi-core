using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to put on an array property in a factory for it to be populated with all boxed instances
    /// </summary>
    [AttributeUsage(AttributeTargets.Property)]
    public class BindingFactoryArrayAttribute : Attribute {
        /// <summary>
        /// Attribute constructor
        /// </summary>
        /// <param name="elementType">The type of element to construct for the array</param>
        public BindingFactoryArrayAttribute(Type elementType) {
        }
    }
}
