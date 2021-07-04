using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.WidgetFactory {
    public interface IWidgetFactoryImpl {
        IEnumerable<IWidgetViewModelFactory> ViewModels { get; }

        Widget InitWidget(string id);

        object CreateWidget(Widget model);
    }
}
