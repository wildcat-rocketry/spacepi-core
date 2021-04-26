using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.BuildTool {
    [AttributeUsage(AttributeTargets.Property)]
    public class ToolArgumentAttribute : Attribute {
        public readonly int Index;

        public ToolArgumentAttribute(int index) {
            Index = index;
        }
    }
}
