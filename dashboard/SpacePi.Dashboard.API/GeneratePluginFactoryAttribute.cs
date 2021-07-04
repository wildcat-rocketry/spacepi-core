using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Class)]
    public class GeneratePluginFactoryAttribute : Attribute {
        public readonly bool GenerateMain;

        public GeneratePluginFactoryAttribute(bool generateMain = false) => GenerateMain = generateMain;
    }
}
