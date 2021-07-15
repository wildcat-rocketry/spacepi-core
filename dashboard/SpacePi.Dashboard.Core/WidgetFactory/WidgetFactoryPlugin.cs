using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WidgetFactory {
    public class WidgetFactoryPlugin : CorePlugin<WidgetFactoryContext> {
        protected override string PluginName => nameof(WidgetFactory);
    }
}
