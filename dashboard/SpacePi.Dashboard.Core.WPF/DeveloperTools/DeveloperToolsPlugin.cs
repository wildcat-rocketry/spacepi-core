using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    [Plugin("SpacePi.Dashboard.Core.WPF.DeveloperTools", "3.0.0", 51_002_000)]
    public class DeveloperToolsPlugin : Plugin, IWindowFactory<Window> {
        [BindPlugin]
        public Core.DeveloperTools.DeveloperToolsPlugin Core { get; set; }

        public DeveloperToolsWindow Window;

        public Window CreateWindow() {
            Window = new DeveloperToolsWindow() {
                DataContext = new[] { Core.Root }
            };
            return Window;
        }

        protected override void Dispose(bool disposing) {
            base.Dispose(disposing);
            if (disposing) {
                Window.Dispatcher.Invoke(() => Window.Close());
            }
        }
    }
}
