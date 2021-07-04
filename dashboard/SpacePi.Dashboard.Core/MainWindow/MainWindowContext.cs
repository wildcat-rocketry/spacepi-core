using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.Core.WidgetFactory;

namespace SpacePi.Dashboard.Core.MainWindow {
    public class MainWindowContext : CoreContext, IGraphicsLoader {
        [BindContext]
        public WidgetFactoryContext WidgetFactory { get; set; }

        public object MainWidget { get; private set; }

        public void LoadGraphics() {
            MainWidget = WidgetFactory.CreateWidget(null); // TODO
        }
    }
}
