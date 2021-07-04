using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.AboutPage {
    [Plugin(100)]
    public class AboutPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(AboutPage);

        [BindPlugin]
        public IEnumerable<IPlugin> Plugins { get; set; }

        public void Load() {
            Trace.WriteLine("Loaded plugins:");
            foreach (IPlugin plugin in Plugins) {
                Trace.WriteLine($"{plugin.Name} v{plugin.Version}");
            }
        }
    }
}
