using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.AboutPage {
    [Plugin("SpacePi.Dashboard.Core.AboutPage", "3.0.0", 100)]
    public class AboutPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(AboutPage);

        [BindPlugin]
        public PluginFactory Factory { get; set; }

        public void Load() {
            Trace.WriteLine("Loaded plugins:");
            foreach ((IPlugin _, string id, string version, int priority) in Factory.Plugins) {
                Trace.WriteLine($"{id} v{version}: {priority}");
            }
        }
    }
}
