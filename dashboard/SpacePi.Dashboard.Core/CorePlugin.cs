using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core {
    public abstract class CorePlugin : IPlugin {
        protected static readonly Version AssemblyVersion = GetAssemblyVersion();
        private bool Disposed;

        protected abstract string PluginName { get; }

        public virtual string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Core)}.{PluginName}";

        public Version Version => AssemblyVersion;

        protected static Version GetAssemblyVersion() => Assembly.GetCallingAssembly().GetName().Version;

        protected virtual void Dispose(bool disposing) {
            if (!Disposed) {
                Disposed = true;
            }
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }

    public abstract class CorePlugin<TInstance> : CorePlugin, IPlugin<TInstance> where TInstance : CorePluginInstance, new() {
        public virtual TInstance CreateInstance() => new();
    }
}
