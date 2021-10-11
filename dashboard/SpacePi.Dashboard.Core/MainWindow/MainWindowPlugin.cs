using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.Core.WidgetFactory;

[assembly: WindowMenuItem("SpacePi.Dashboard.Core.MainWindow.ExitMenu", "Exit", 2_000_000_000, "SpacePi.Dashboard.API.FileMenu")]

namespace SpacePi.Dashboard.Core.MainWindow {
    [Plugin("SpacePi.Dashboard.Core.MainWindow", "3.0.0", 11_003_000)]
    public class MainWindowPlugin : Plugin, IGraphicsLoader {
        [BindPlugin]
        public WidgetFactoryPlugin WidgetFactory { get; set; }

        [BindPlugin]
        public IEnumerable<IPlugin> Plugins { get; set; }

        [BindWindowMenu("SpacePi.Dashboard.Core.MainWindow.ExitMenu")]
        public WindowMenuItem ExitMenuItem { get; set; }

        public object MainWidget { get; private set; }

        public void ExitApplication() {
            foreach (IPlugin plugin in Plugins) {
                plugin.Dispose();
            }
        }

        public void LoadGraphics() {
            MainWidget = WidgetFactory.CreateWidget(null); // TODO
        }
    }
}
