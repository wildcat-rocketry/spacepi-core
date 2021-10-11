using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    [SuperPlugin("SpacePi.Dashboard.Core.WPF.WPFThread", "3.0.0", 1_000_000_000)]
    public class WPFThreadSuperPlugin : Plugin {
        public Thread Thread { get; }

        internal readonly bool[] IsThreadStarted = new[] { false };

        public App App { get; private set; }

        [BindSuperPlugin]
        public IEnumerable<IGraphicsLoader> GraphicsLoaders { get; set; }

        public override void Load() {
            Thread.Start();
            lock (IsThreadStarted) {
                while (!IsThreadStarted[0]) {
                    Monitor.Wait(IsThreadStarted);
                }
            }
            App.Dispatcher.Invoke(() => {
                foreach (IGraphicsLoader loader in GraphicsLoaders) {
                    loader.LoadGraphics();
                }
            });
        }

        public WPFThreadSuperPlugin() {
            Thread = new(() => {
                App = new() {
                    Signal = IsThreadStarted
                };
                App.InitializeComponent();
                App.Run();
            });
            Thread.Name = "WPF Thread";
            Thread.SetApartmentState(ApartmentState.STA);
        }
    }
}
