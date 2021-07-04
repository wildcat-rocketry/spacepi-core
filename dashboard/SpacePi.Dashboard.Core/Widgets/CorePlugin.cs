using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.Widgets {
    public abstract class CorePlugin<TWidget> : Core.CorePlugin<TWidget> where TWidget : CoreContext, new() {
        public override string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Core)}.{nameof(Widgets)}.{PluginName}";
    }
}
