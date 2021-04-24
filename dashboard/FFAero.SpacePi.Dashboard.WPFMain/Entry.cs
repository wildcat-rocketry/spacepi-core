using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FFAero.SpacePi.Dashboard.WPF;

namespace FFAero.SpacePi.Dashboard.WPFMain {
    static class Entry {
        [STAThread]
        public static void Main() {
            App app = new();
            app.InitializeComponent();
            app.Run();
        }
    }
}
