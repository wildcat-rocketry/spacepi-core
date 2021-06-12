using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.AboutPage {
    [Plugin]
    public class AboutPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(AboutPage);

        public void Load() {
            Trace.WriteLine("Loaded plugins:");
            foreach (IPlugin plugin in IPluginFactory.Instance.Plugins) {
                Trace.WriteLine(string.Format("{0} v{1}", plugin.Name, plugin.Version));
            }
        }
    }
}
