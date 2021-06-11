using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public interface IPluginInstance : IDisposable {
        IEnumerable<object> RegisterSettings() => Enumerable.Empty<object>();

        IEnumerable<INetworkAdapter> RegisterNetworkAdapters() => Enumerable.Empty<INetworkAdapter>();

        IEnumerable<IViewComponent> RegisterViewComponents() => Enumerable.Empty<IViewComponent>();
    }
}
