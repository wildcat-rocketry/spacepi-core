using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to put on a declarative attribute constructor for the name of the key for a dictionary-type binding
    /// </summary>
    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingDictionaryKeyAttribute : Attribute {
    }
}
