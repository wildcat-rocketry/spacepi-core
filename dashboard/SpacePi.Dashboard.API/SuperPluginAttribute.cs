﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Class)]
    public class SuperPluginAttribute : Attribute {
        public SuperPluginAttribute(
            [BindingID] string id,
            [BindingParameter("version")] string version,
            [BindingPriority] int priority) {
        }
    }
}
