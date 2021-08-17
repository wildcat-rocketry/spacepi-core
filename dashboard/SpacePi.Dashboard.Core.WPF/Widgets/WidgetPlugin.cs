using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.Widgets {
    public abstract class WidgetPlugin<TViewModelFactory> : Plugin, IWidgetFactory<UserControl> where TViewModelFactory : IPlugin, IWidgetViewModelFactory {
        [BindPlugin]
        public TViewModelFactory ViewModelFactory { get; set; }

        IWidgetViewModelFactory IWidgetFactory<UserControl>.ViewModelFactory => ViewModelFactory;

        public abstract UserControl CreateWidget();

        public UserControl CreateWidget(object viewModel) {
            UserControl widget = CreateWidget();
            widget.DataContext = viewModel;
            return widget;
        }
    }
}
