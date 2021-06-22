using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using SpacePi.Dashboard.API;
using SpacePi.Dashboard.API.Startup;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    public class WPFThreadContext : CoreContext, IContext {
        [BindContext]
        public StartupContext Startup;

        [BindPlugin]
        public WPFThreadPlugin Plugin;

        public void Load() => Plugin.App.Dispatcher.Invoke(() => {
            foreach (IWindowFactory<Window> win in Startup.ContextFactory.Contexts.OfType<IWindowFactory<Window>>()) {
                win.CreateWindow().Show();
            }
        });
    }
}
