using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to cause a main function to be generated which constructs any factories marked with [EntryPoint]
    /// </summary>
    [AttributeUsage(AttributeTargets.Assembly)]
    public class GenerateBindingsAttribute : Attribute {
    }
}
