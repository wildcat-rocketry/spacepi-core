using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace SpacePi.Dashboard.Core.WPF.Widgets.DebugBlock {
    public class DebugBlockContext : CoreContext<Core.Widgets.DebugBlock.DebugBlockContext> {
        public override UserControl CreateWidget() => new DebugBlockControl();
    }
}
