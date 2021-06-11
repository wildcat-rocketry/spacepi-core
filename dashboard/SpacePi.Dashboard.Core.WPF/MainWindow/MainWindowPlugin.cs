using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    [Plugin]
    public class MainWindowPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(MainWindow);

        public void Load() {
            Thread t = new(() => {
                App app = new();
                app.InitializeComponent();
                app.Run();
            });
            t.Name = "WPF Thread";
            t.SetApartmentState(ApartmentState.STA);
            t.Start();
        }
    }
}
