using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Startup {
    public class StartupContext : IContext {
        public IContextFactory ContextFactory { get; internal set; }

        public void Dispose() => GC.SuppressFinalize(this);
    }
}
