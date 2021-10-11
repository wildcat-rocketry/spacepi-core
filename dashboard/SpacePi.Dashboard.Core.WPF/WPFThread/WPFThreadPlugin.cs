using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    [Plugin("SpacePi.Dashboard.Core.WPF.WPFThread", "3.0.0", 2_000_200_000)]
    public class WPFThreadPlugin : Plugin {
        [BindSuperPlugin]
        public WPFThreadSuperPlugin SuperPlugin { get; set; }

        [BindPlugin]
        public IEnumerable<IGraphicsLoader> GraphicsLoaders { get; set; }

        [BindPlugin]
        public IEnumerable<IWindowFactory<Window>> WindowFactories { get; set; }

        public override void Load() {
            SuperPlugin.App.Dispatcher.Invoke(() => {
                foreach (IGraphicsLoader loader in GraphicsLoaders) {
                    loader.LoadGraphics();
                }
                foreach (IWindowFactory<Window> factory in WindowFactories) {
                    factory.CreateWindow().Show();
                }
            });
        }
    }
}
