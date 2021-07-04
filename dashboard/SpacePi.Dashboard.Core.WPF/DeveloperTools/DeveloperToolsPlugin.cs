using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    [Plugin(210)]
    public class DeveloperToolsPlugin : CorePlugin<DeveloperToolsContext> {
        protected override string PluginName => nameof(DeveloperTools);
    }
}
