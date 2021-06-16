using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public interface IPlugin : IDisposable {
        string Name { get; }

        Version Version { get; }

        void Load() { }
    }

    public interface IPlugin<TInstance> : IPlugin where TInstance : IContext {
        TInstance CreateContext();
    }
}
