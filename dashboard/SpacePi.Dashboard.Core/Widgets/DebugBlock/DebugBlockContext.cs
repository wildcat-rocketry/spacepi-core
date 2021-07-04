using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.Widgets.DebugBlock {
    public class DebugBlockContext : CoreContext {
        public override string Name => "Debug Block";

        protected override string PluginName => nameof(DebugBlock);

        public override object CreateViewModel(Widget model) => null;
    }
}
