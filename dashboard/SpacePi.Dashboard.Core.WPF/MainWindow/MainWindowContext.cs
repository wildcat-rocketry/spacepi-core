using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Startup;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    public class MainWindowContext : CoreContext, IWindowFactory<Window> {
        [BindContext]
        public StartupContext Startup;

        public Window CreateWindow() => new MainWindow(Startup.ContextFactory);
    }
}
