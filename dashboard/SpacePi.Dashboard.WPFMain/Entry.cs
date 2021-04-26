using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer;
using SpacePi.Dashboard.WPF;

namespace SpacePi.Dashboard.WPFMain {
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
