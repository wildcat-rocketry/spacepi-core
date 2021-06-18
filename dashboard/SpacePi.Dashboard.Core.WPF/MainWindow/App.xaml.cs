using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    public partial class App : Application {
        public IContextFactory ContextFactory;

        public UserControl RootComponent;

        private void AppStartup(object sender, StartupEventArgs e) => new MainWindow(ContextFactory, RootComponent).Show();
    }
}
