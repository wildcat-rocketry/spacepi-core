using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// A user-defined parameter which can be set on a declarative attribute for factory objects
    /// </summary>
    public record Parameter {
        /// <summary>
        /// The type of parameter to the boxing function
        /// </summary>
        public enum Types {
            /// <summary>
            /// The parameter is an instance of the target type
            /// </summary>
            FactoryObject,
            /// <summary>
            /// The ID of the instance
            /// </summary>
            ID,
            /// <summary>
            /// A user-defined parameter defined in the declarative attribute
            /// </summary>
            Parameter,
            /// <summary>
            /// The priority of the instance
            /// </summary>
            Priority
        }

        /// <summary>
        /// The parameter in the attribute
        /// </summary>
        public IParameterSymbol Symbol;
        /// <summary>
        /// The type of parameter
        /// </summary>
        public Types Type;
        /// <summary>
        /// The name of the parameter
        /// </summary>
        public string Name;
    }
}
