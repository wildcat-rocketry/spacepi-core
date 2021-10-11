using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.WidgetFactory {
    public class WidgetFactoryImpl<TGfxWidget> : Plugin, IWidgetFactoryImpl {
        [BindPlugin("id")]
        public IDictionary<string, IWidgetFactory<TGfxWidget>> Widgets { get; set; }

        [BindPlugin]
        public WidgetFactoryPlugin Core { get; set; }

        public IEnumerable<IWidgetViewModelFactory> ViewModels => Widgets.Select(p => p.Value.ViewModelFactory);

        public object CreateWidget(Widget model) {
            IWidgetFactory<TGfxWidget> factory = Widgets.Values.First(); // TODO
            return factory.CreateWidget(factory.ViewModelFactory.CreateViewModel(model));
        }

        public Widget InitWidget(string id) => new(); // TODO

        public override void Load() => Core.Init(this);
    }
}
