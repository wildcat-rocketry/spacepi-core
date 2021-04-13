using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FFAero.SpacePi.Dashboard.API {
    public interface IPluginInstance : IDisposable {
        IEnumerable<object> RegisterSettings() => Array.Empty<object>();

        IEnumerable<INetworkAdapter> RegisterNetworkAdapters() => Array.Empty<INetworkAdapter>();

        IEnumerable<IViewComponent> RegisterViewComponents() => Array.Empty<IViewComponent>();
    }
}
