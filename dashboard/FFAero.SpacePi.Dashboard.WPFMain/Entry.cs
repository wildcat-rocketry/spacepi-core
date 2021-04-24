using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FFAero.SpacePi.Dashboard.Analyzer;
using FFAero.SpacePi.Dashboard.WPF;

namespace FFAero.SpacePi.Dashboard.WPFMain {
    static class Entry {
        [STAThread]
        public static void Main() {
            PluginContext.Setup();
            App app = new();
            app.InitializeComponent();
            app.Run();
        }
    }
}
