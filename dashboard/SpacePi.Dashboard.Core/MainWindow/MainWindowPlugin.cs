using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.Core.WidgetFactory;

namespace SpacePi.Dashboard.Core.MainWindow {
    [Plugin("SpacePi.Dashboard.Core.MainWindow", "3.0.0", 11_003_000)]
    public class MainWindowPlugin : Plugin, IGraphicsLoader {
        [BindPlugin]
        public WidgetFactoryPlugin WidgetFactory { get; set; }

        public object MainWidget { get; private set; }

        public void LoadGraphics() {
            MainWidget = WidgetFactory.CreateWidget(null); // TODO
        }
    }
}
