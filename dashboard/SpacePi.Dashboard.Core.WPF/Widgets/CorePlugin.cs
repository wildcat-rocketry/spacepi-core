using SpacePi.Dashboard.API;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace SpacePi.Dashboard.Core.WPF.Widgets {
    public abstract class CorePlugin<TContext> : WPF.CorePlugin<TContext> where TContext : CoreContext, IWidgetFactory<UserControl>, new() {
        public override string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Core)}.{nameof(WPF)}.{nameof(Widgets)}.{PluginName}";
    }
}
