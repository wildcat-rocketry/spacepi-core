using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.WPF {
    public abstract class CorePlugin : Core.CorePlugin {
        public override string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Core)}.{nameof(WPF)}.{PluginName}";

        static CorePlugin() {
            if (AssemblyVersion != GetAssemblyVersion()) {
                throw new NotSupportedException($"{nameof(Core)} <-> {nameof(Core)}.{nameof(WPF)} version mismatch");
            }
        }
    }
}
