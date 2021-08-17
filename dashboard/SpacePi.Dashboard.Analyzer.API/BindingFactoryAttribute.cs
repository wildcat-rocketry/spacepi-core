using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark the a factory for the binding analyzer
    /// </summary>
    [AttributeUsage(AttributeTargets.Class)]
    public class BindingFactoryAttribute : Attribute {
        /// <summary>
        /// Attribute constructor
        /// </summary>
        /// <param name="declarativeAttribute">The type of attribute used to declare an instance created by the factory</param>
        /// <param name="bindingAttribute">The type of attribute used to bind a property using this factory</param>
        public BindingFactoryAttribute(Type declarativeAttribute, Type bindingAttribute) {
        }

        /// <summary>
        /// Attribute constructor
        /// </summary>
        /// <param name="declarativeAttribute">The type of attribute used to declare an instance created by the factory</param>
        /// <param name="bindingAttribute">The type of attribute used to bind a property using this factory</param>
        /// <param name="defaultType">The default type when a binding attribute is used on an assembly</param>
        public BindingFactoryAttribute(Type declarativeAttribute, Type bindingAttribute, Type defaultType) {
        }
    }
}
