using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.WidgetFactory {
    public class WidgetFactoryContext : CoreContext, IWidgetFactoryImpl {
        private IWidgetFactoryImpl Impl;

        [BindContext]
        public IEnumerable<IWidgetFactory> WidgetFactories { get; set; }

        public void Init<TGfxWidget>() => Impl = new WidgetFactoryImpl<TGfxWidget>(WidgetFactories.OfType<IWidgetFactory<TGfxWidget>>());

        public IEnumerable<IWidgetViewModelFactory> ViewModels => Impl.ViewModels;

        public object CreateWidget(Widget model) => Impl.CreateWidget(model);

        public Widget InitWidget(string id) => Impl.InitWidget(id);
    }
}
