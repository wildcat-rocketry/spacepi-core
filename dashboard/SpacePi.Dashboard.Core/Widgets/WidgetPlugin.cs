using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.Widgets {
    public abstract class WidgetPlugin : Plugin, IWidgetViewModelFactory {
        public abstract string Name { get; }

        public abstract object CreateViewModel(Widget model);

        public virtual void InitSettings(Widget model) {
        }
    }
}
