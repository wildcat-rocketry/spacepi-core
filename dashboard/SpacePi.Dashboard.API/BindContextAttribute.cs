﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
    public class BindContextAttribute : Attribute {
    }
}
