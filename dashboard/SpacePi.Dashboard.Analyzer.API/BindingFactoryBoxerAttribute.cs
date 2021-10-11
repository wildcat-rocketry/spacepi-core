using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a method as the boxing method for the factory
    /// </summary>
    [AttributeUsage(AttributeTargets.Method)]
    public class BindingFactoryBoxerAttribute : Attribute {
    }
}
