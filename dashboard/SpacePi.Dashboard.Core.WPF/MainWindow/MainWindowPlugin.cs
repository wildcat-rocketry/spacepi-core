using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.MainWindow {
    [Plugin(1000010)]
    public class MainWindowPlugin : CorePlugin<MainWindowContext> {
        protected override string PluginName => nameof(MainWindow);
    }
}
