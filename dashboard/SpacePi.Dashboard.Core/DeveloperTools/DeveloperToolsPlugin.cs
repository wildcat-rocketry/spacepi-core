using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    [Plugin]
    public class DeveloperToolsPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(DeveloperTools);
    }
}
