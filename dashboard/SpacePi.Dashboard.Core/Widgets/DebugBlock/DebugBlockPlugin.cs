using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.Widgets.DebugBlock {
    [Plugin(300)]
    public class DebugBlockPlugin : CorePlugin<DebugBlockContext> {
        protected override string PluginName => nameof(DebugBlock);
    }
}
