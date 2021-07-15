using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    [Plugin("SpacePi.Dashboard.Core.WPF.MainWindow", "3.0.0", 51_003_000)]
    public class MainWindowPlugin : Plugin, IWindowFactory<Window> {
        [BindPlugin]
        public IEnumerable<IPlugin> Plugins { get; set; }

        [BindPlugin]
        public Core.MainWindow.MainWindowPlugin Core { get; set; }

        public Window CreateWindow() => new MainWindow(Plugins) {
            DataContext = Core
        };
    }
}
