using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FFAero.SpacePi.Dashboard.API;

namespace FFAero.SpacePi.Dashboard.Core.AboutPage {
    [Plugin]
    public class AboutPlugin : IPlugin {
        public string Name => "FFAero.SpacePi.Dashboard.Core.AboutPage";

        public Version Version => AssemblyInfo.Version;

        public IEnumerable<object> RegisterSettings() {
            Trace.WriteLine("Loaded plugins:");
            foreach (IPlugin plugin in PluginFactory.LoadedPlugins) {
                Trace.WriteLine(string.Format("{0} v{1}", plugin.Name, plugin.Version));
            }
            return Array.Empty<object>();
        }

        public void Load() {
            Trace.WriteLine("Plugin Loaded.");
        }
    }
}
