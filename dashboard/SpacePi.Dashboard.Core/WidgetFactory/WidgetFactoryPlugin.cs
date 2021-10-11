using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.WidgetFactory {
    [Plugin("SpacePi.Dashboard.Core.WidgetFactory", "3.0.0", 11_004_000)]
    public class WidgetFactoryPlugin : Plugin, IWidgetFactoryImpl {
        private IWidgetFactoryImpl Impl;

        public void Init(IWidgetFactoryImpl impl) => Impl = impl;

        public IEnumerable<IWidgetViewModelFactory> ViewModels => Impl.ViewModels;

        public object CreateWidget(Widget model) => Impl.CreateWidget(model);

        public Widget InitWidget(string id) => Impl.InitWidget(id);
    }
}
