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
        public Thread Thread { get; }

        internal readonly bool[] IsThreadStarted = new[] { false };

        public App App { get; private set; }

        [BindPlugin]
        public IEnumerable<IGraphicsLoader> GraphicsLoaders { get; set; }

        [BindPlugin]
        public IEnumerable<IWindowFactory<Window>> WindowFactories { get; set; }

        public override void Load() {
            Thread.Start();
            lock (IsThreadStarted) {
                while (!IsThreadStarted[0]) {
                    Monitor.Wait(IsThreadStarted);
                }
            }
        }

        public WPFThreadPlugin() {
            Thread = new(() => {
                App = new() {
                    Plugin = this
                };
                App.InitializeComponent();
                App.Run();
            });
            Thread.Name = "WPF Thread";
            Thread.SetApartmentState(ApartmentState.STA);
        }
    }
}
