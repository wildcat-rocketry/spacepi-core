using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.AboutPage {
    [Plugin("SpacePi.Dashboard.Core.AboutPage", "3.0.0", 11_001_000)]
    public class AboutPlugin : Plugin {
        [BindPlugin]
        public PluginFactory Factory { get; set; }

        public override void Load() {
            Trace.WriteLine("");
            int idMax = new[] { "Plugin" }.Concat(Factory.Plugins.Select(t => t.ID)).Max(s => s.Length);
            int verMax = new[] { "Version" }.Concat(Factory.Plugins.Select(t => t.Version)).Max(s => s.Length);
            Trace.WriteLine(string.Format($"| Priority       | {{0,-{idMax}}} | {{1,-{verMax}}} |", "Plugin", "Version"));
            Trace.WriteLine($"|----------------|-{"".PadRight(idMax, '-')}-|-{"".PadRight(verMax, '-')}-|");
            foreach ((IPlugin _, string id, string version, int priority) in Factory.Plugins) {
                Trace.WriteLine(string.Format($"| {priority,14:N0} | {{0,-{idMax}}} | {{1,-{verMax}}} |", id, version));
            }
            Trace.WriteLine("");
        }
    }
}
