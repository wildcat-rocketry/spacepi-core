using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    [Plugin]
    public class DeveloperToolsPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(DeveloperTools);

        [BindPlugin]
        public Core.DeveloperTools.DeveloperToolsPlugin CorePlugin;

        public void Load() {
            Trace.WriteLine($"CorePlugin: {CorePlugin}");
        }
    }
}
