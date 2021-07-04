using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    public class MainWindowContext : CoreContext, IWindowFactory<Window> {
        [BindContext]
        public IEnumerable<IContext> Contexts { get; set; }

        [BindContext]
        public Core.MainWindow.MainWindowContext Core { get; set; }

        public Window CreateWindow() => new MainWindow(Contexts) {
            DataContext = Core
        };
    }
}
