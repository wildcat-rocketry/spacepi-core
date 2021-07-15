using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.Core.WidgetFactory;

namespace SpacePi.Dashboard.Core.WPF.WidgetFactory {
    [Plugin("SpacePi.Dashboard.Core.WPF.WidgetFactory", "3.0.0", 2_000_100_000)]
    public class WidgetFactoryPlugin : WidgetFactoryImpl<UserControl> {
    }
}
