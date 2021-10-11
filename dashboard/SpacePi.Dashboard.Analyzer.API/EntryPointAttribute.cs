using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a factory to be constructed from the main function
    /// </summary>
    [AttributeUsage(AttributeTargets.Class)]
    public class EntryPointAttribute : Attribute {
    }
}
