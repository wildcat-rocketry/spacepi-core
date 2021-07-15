using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.API {
    public interface IWidgetViewModelFactory {
        string Id { get; }

        string Name { get; }

        void InitSettings(Widget model);

        object CreateViewModel(Widget model);
    }
}
