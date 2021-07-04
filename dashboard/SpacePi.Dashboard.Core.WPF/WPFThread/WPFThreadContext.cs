using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    public class WPFThreadContext : CoreContext, IContext {
        [BindContext]
        public IEnumerable<IGraphicsLoader> GraphicsLoaders { get; set; }

        [BindContext]
        public IEnumerable<IWindowFactory<Window>> WindowFactories { get; set; }

        [BindPlugin]
        public WPFThreadPlugin Plugin { get; set; }

        public void Load() => Plugin.App.Dispatcher.Invoke(() => {
            foreach (IGraphicsLoader loader in GraphicsLoaders) {
                loader.LoadGraphics();
            }
            foreach (IWindowFactory<Window> win in WindowFactories) {
                win.CreateWindow().Show();
            }
        });
    }
}
