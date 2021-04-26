using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public interface IPlugin {
        string Name { get; }

        Version Version { get; }

        IEnumerable<object> RegisterSettings() => Array.Empty<object>();

        void Load() { }

        IPluginInstance NewInstance(object model) => null;
    }
}
