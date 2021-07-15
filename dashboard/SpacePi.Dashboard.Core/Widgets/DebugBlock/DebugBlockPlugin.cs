using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.Widgets.DebugBlock {
    [Plugin("SpacePi.Dashboard.Core.Widgets.DebugBlock", "3.0.0", 11_005_000)]
    public class DebugBlockPlugin : WidgetPlugin {
        public override string Name => "Debug Block";

        public override object CreateViewModel(Widget model) => null;
    }
}
