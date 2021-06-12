using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public interface IPlugin {
        string Name { get; }

        Version Version { get; }

        void Load() { }

        IPluginInstance CreateInstance() => null;
    }
}
