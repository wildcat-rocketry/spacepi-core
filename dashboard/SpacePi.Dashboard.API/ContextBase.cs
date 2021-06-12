using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public abstract class ContextBase : IContext {
        protected abstract IPluginInstance[] PluginsArray { get; }

        public IEnumerable<IPluginInstance> Plugins => PluginsArray.Where(p => p != null);

        public void Dispose() {
            foreach (IPluginInstance plugin in Plugins) {
                plugin.Dispose();
            }
            GC.SuppressFinalize(this);
        }
    }
}
