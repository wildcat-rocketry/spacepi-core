using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingIDAttribute : Attribute {
    }
}
