using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.WidgetFactory {
    class WidgetFactoryImpl<TGfxWidget> : IWidgetFactoryImpl {
        private readonly Dictionary<string, IWidgetFactory<TGfxWidget>> Widgets;

        public IEnumerable<IWidgetViewModelFactory> ViewModels => Widgets.Select(p => p.Value.ViewModelFactory);

        public object CreateWidget(Widget model) {
            IWidgetFactory<TGfxWidget> factory = Widgets.Values.First(); // TODO
            return factory.CreateWidget(factory.ViewModelFactory.CreateViewModel(model));
        }

        public Widget InitWidget(string id) => new(); // TODO

        public WidgetFactoryImpl(IEnumerable<IWidgetFactory<TGfxWidget>> ctx) => Widgets = ctx.ToDictionary(w => w.ViewModelFactory.Id);
    }
}
