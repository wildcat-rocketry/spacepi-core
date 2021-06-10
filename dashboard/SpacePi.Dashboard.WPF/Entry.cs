using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer;
using SpacePi.Dashboard.Core.WPF;

namespace SpacePi.Dashboard.WPF {
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
