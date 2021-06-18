using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Startup;
using SpacePi.Dashboard.Core.WPF.DeveloperTools;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    public class MainWindowContext : CoreContext, IContext {
        [BindContext]
        public StartupContext Startup;

        [BindContext]
        public DeveloperToolsContext DeveloperTools;

        public void Load() {
            Thread t = new(() => {
                App app = new();
                app.ContextFactory = Startup.ContextFactory;
                app.RootComponent = DeveloperTools.ViewComponent;
                app.InitializeComponent();
                app.Run();
            });
            t.Name = "WPF Thread";
            t.SetApartmentState(ApartmentState.STA);
            t.Start();
        }
    }
}
