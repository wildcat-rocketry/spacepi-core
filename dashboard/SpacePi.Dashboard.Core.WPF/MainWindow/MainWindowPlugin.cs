using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    [Plugin]
    public class MainWindowPlugin : IPlugin {
        public string Name => "SpacePi.Dashboard.Core.WPF.MainWindow";

        public Version Version => AssemblyInfo.Version;

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
