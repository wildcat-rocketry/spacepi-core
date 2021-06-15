using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF {
    public abstract class CorePlugin : Core.CorePlugin {
        public override string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Core)}.{nameof(WPF)}.{PluginName}";

        internal static void CheckVersion() {
            if (AssemblyVersion != GetAssemblyVersion()) {
                throw new NotSupportedException($"{nameof(Core)} <-> {nameof(Core)}.{nameof(WPF)} version mismatch");
            }
        }

        static CorePlugin() => CheckVersion();
    }

    public abstract class CorePlugin<TContext> : Core.CorePlugin<TContext> where TContext : CoreContext, new() {
        static CorePlugin() => CorePlugin.CheckVersion();
    }
}
