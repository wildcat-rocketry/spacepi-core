using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Assembly, AllowMultiple = true)]
    public class WindowMenuItemAttribute : Attribute {
        public WindowMenuItemAttribute(
            [BindingID, BindingParameter("id")] string id,
            [BindingParameter("name")] string name,
            [BindingPriority] int priority,
            [BindingParameter("parent")] string parent) {
        }
    }
}
