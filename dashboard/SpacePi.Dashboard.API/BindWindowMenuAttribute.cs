using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Property)]
    public class BindWindowMenuAttribute : Attribute {
        public BindWindowMenuAttribute([BindingID] string id) {
        }

        public BindWindowMenuAttribute(bool tag, [BindingMatchID] string prop) {
        }
    }
}
