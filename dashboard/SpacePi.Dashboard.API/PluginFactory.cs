using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public static class PluginFactory {
        public static IEnumerable<IPlugin> LoadedPlugins { get; private set; }

        public static IEnumerable<object> GlobalSettings { get; private set; }

        public static void Initialize(IEnumerable<IPlugin> plugins) {
            LoadedPlugins = plugins.ToImmutableArray();
            List<object> globalSettings = new();
            foreach (IPlugin plugin in LoadedPlugins) {
                // TODO
                plugin.RegisterSettings();
            }
            foreach (IPlugin plugin in LoadedPlugins) {
                plugin.Load();
            }
            GlobalSettings = globalSettings.ToImmutableArray();
        }
    }
}
