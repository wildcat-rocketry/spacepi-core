using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    public partial class App : Application {
        internal WPFThreadPlugin Plugin { private get; init; }

        private void AppStartup(object sender, StartupEventArgs e) {
            lock (Plugin.IsThreadStarted) {
                Plugin.IsThreadStarted[0] = true;
                foreach (IGraphicsLoader loader in Plugin.GraphicsLoaders) {
                    loader.LoadGraphics();
                }
                Monitor.PulseAll(Plugin.IsThreadStarted);
            }
        }
    }
}
