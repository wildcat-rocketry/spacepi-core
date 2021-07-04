using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.Widgets.DebugBlock {
    [Plugin(310)]
    public class DebugBlockPlugin : CorePlugin<DebugBlockContext> {
        protected override string PluginName => nameof(DebugBlock);
    }
}
