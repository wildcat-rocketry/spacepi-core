using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API {
    public class Plugin : IPlugin {
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

        public virtual void Load() {
        }
    }
}
