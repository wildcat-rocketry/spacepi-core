using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [BindingFactory(typeof(PluginAttribute), typeof(BindPluginAttribute))]
    public class PluginFactory {
        [BindingFactoryArray(typeof((IPlugin, string, string, int)))]
        public (IPlugin Plugin, string ID, string Version, int Priority)[] Plugins { get; set; }

        [BindingFactoryBoxer]
        public (IPlugin, string, string, int) Box(
            [BindingFactoryObject] IPlugin plugin,
            [BindingID] string id,
            [BindingParameter("version")] string version,
            [BindingPriority] int priority) => (plugin, id, version, priority);

        [BindingFactoryUnboxer]
        public IPlugin Unbox((IPlugin plugin, string _1, string _2, int _3) p) => p.plugin;
        
        [BindingFactoryLoader]
        public void Load() {
            foreach ((IPlugin plugin, string _, string _, int _) in Plugins) {
                plugin.Load();
            }
        }
    }
}
