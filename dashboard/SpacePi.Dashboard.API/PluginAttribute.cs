using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Class)]
    public class PluginAttribute : Attribute {
        public readonly int Priority;

        public PluginAttribute(int priority = 0) => Priority = priority;
    }
}
