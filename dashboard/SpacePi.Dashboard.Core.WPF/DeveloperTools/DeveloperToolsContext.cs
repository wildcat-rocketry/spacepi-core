using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.DeveloperTools {
    public class DeveloperToolsContext : CoreContext, IWindowFactory<Window> {
        [BindContext]
        public Core.DeveloperTools.DeveloperToolsContext Core { get; set; }

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
