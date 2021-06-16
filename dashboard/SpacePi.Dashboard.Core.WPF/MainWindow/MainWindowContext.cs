using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Startup;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    public class MainWindowContext : CoreContext, IContext {
        [BindContext]
        public StartupContext Startup;

        public void Load() {
            Thread t = new(() => {
                App app = new();
                app.ContextFactory = Startup.ContextFactory;
                app.InitializeComponent();
                app.Run();
            });
            t.Name = "WPF Thread";
            t.SetApartmentState(ApartmentState.STA);
            t.Start();
        }
    }
}
