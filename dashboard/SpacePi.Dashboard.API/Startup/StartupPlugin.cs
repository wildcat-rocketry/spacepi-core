using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Startup {
    [Plugin(int.MinValue)]
    public class StartupPlugin : IPlugin<StartupContext> {
        public string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(API)}.{nameof(Startup)}";

        public Version Version => Assembly.GetExecutingAssembly().GetName().Version;

        public IPluginFactory PluginFactory { get; private set; }

        public IContextFactory InitialContext { get; private set; }

        public void Dispose() => GC.SuppressFinalize(this);

        StartupContext IPlugin<StartupContext>.CreateContext() => new();

        public IContextFactory CreateContext() {
            IContextFactory factory = PluginFactory.CreateContext();
            ((StartupContext) factory.Contexts[0]).ContextFactory = factory;
            foreach (IContext ctx in factory.Contexts) {
                ctx.Load();
            }
            return factory;
        }

        public static void Start(IPluginFactory factory) {
            StartupPlugin startup = (StartupPlugin) factory.Plugins[0];
            startup.PluginFactory = factory;
            foreach (IPlugin plugin in factory.Plugins) {
                plugin.Load();
            }
            startup.InitialContext = startup.CreateContext();
        }
    }
}
