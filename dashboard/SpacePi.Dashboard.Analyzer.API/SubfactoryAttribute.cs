using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// Attribute used to obtain a child factory instance from a factory
    /// </summary>
    [AttributeUsage(AttributeTargets.Property)]
    public class SubfactoryAttribute : Attribute {
    }
}
