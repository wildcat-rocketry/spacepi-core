using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WidgetFactory {
    [Plugin("SpacePi.Dashboard.Core.WPF.WidgetFactory", "3.0.0", 2_000_100_000)]
    public class WidgetFactoryPlugin : Plugin {
        [BindPlugin]
        public Core.WidgetFactory.WidgetFactoryPlugin Core { get; set; }

        public override void Load() => Core.Init<UserControl>();
    }
}
