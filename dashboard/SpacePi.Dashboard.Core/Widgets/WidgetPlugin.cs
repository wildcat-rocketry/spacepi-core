using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Model;

namespace SpacePi.Dashboard.Core.Widgets {
    public abstract class WidgetPlugin : Plugin, IWidgetViewModelFactory {
        protected abstract string PluginName { get; }

        public virtual string Id => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Core)}.{nameof(Widgets)}.{PluginName}";

        public abstract string Name { get; }

        public abstract object CreateViewModel(Widget model);

        public virtual void InitSettings(Widget model) {
        }
    }
}
