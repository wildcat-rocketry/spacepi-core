using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WidgetFactory {
    public class WidgetFactoryContext : CoreContext, IContext {
        [BindContext]
        public Core.WidgetFactory.WidgetFactoryContext Core { get; set; }

        public void Load() => Core.Init<UserControl>();
    }
}
