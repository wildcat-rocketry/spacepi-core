using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.WPF.WPFThread {
    [Plugin]
    public class WPFThreadPlugin : CorePlugin<WPFThreadContext>, IPlugin {
        protected override string PluginName => nameof(WPFThread);

        public Thread Thread { get; }

        private readonly bool[] IsThreadStarted = new[] { false };

        public App App { get; private set; }

        public void Load() {
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
                    IsThreadStarted = IsThreadStarted
                };
                App.InitializeComponent();
                App.Run();
            });
            Thread.Name = "WPF Thread";
            Thread.SetApartmentState(ApartmentState.STA);
        }
    }
}
