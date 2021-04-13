using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FFAero.SpacePi.Dashboard.API {
    public class PluginContext : IDisposable {
        private readonly IEnumerable<IPluginInstance> Plugins;

        public readonly object Model;

        public readonly IDictionary<string, INetworkAdapter> NetworkAdapters;

        public readonly IDictionary<string, IViewComponent> ViewComponents;

        public void Dispose() {
            foreach (IPluginInstance plugin in Plugins) {
                plugin.Dispose();
            }
            GC.SuppressFinalize(this);
        }

        public PluginContext(object model) {
            Model = model;
            Plugins = PluginFactory.LoadedPlugins
                .Select(p => p.NewInstance(model))
                .Where(p => p != null)
                .ToImmutableList();
            NetworkAdapters = Plugins
                .SelectMany(p => p.RegisterNetworkAdapters())
                .ToImmutableDictionary(n => n.Key);
            ViewComponents = Plugins
                .SelectMany(p => p.RegisterViewComponents())
                .ToImmutableDictionary(n => n.Key);
            foreach (IPluginInstance plugin in Plugins) {
                // TODO
                plugin.RegisterSettings();
            }
        }
    }
}
