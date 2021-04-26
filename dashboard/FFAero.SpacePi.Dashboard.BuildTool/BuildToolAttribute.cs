using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FFAero.SpacePi.Dashboard.BuildTool {
    [AttributeUsage(AttributeTargets.Class)]
    public class BuildToolAttribute : Attribute {
        public readonly string Name;

        public BuildToolAttribute(string name) {
            Name = name;
        }
    }
}
