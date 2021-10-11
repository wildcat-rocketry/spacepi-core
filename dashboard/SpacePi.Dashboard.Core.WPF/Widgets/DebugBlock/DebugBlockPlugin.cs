using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.Widgets.DebugBlock {
    [Plugin("SpacePi.Dashboard.Core.WPF.Widgets.DebugBlock", "3.0.0", 51_005_000)]
    public class DebugBlockPlugin : WidgetPlugin<Core.Widgets.DebugBlock.DebugBlockPlugin> {
        public override UserControl CreateWidget() => new DebugBlockControl();
    }
}
