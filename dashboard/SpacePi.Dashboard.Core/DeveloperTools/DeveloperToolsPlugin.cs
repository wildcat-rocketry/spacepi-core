﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    public class DeveloperToolsPlugin : CorePlugin<DeveloperToolsContext> {
        protected override string PluginName => nameof(DeveloperTools);
    }
}
