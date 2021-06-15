using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core {
    public abstract class CorePluginInstance : IPluginInstance {
        private bool Disposed;

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
}
