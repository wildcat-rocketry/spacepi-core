﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Property)]
    public class BindSuperPluginAttribute : Attribute {
        public BindSuperPluginAttribute() {
        }

        public BindSuperPluginAttribute([BindingDictionaryKey] string key) {
        }
    }
}
