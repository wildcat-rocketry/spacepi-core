using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Core.AboutPage {
    [Plugin("SpacePi.Dashboard.Core.AboutPage", "3.0.0", 11_001_000)]
    public class AboutPlugin : Plugin {
        [BindSuperPlugin]
        public SuperPluginFactory SuperPlugins { get; set; }

        [BindPlugin]
        public PluginFactory Plugins { get; set; }

        private void PrintTable(StringBuilder str, PluginFactory factory) {
            int idMax = new[] { "Plugin" }.Concat(factory.Plugins.Select(t => t.ID)).Max(s => s.Length);
            int verMax = new[] { "Version" }.Concat(factory.Plugins.Select(t => t.Version)).Max(s => s.Length);
            str.AppendFormat($"| Priority       | {{0,-{idMax}}} | {{1,-{verMax}}} |\n|----------------|-{"".PadRight(idMax, '-')}-|-{"".PadRight(verMax, '-')}-|", "Plugin", "Version");
            foreach ((IPlugin _, string id, string version, int priority) in factory.Plugins) {
                str.AppendFormat($"\n| {priority,14:N0} | {{0,-{idMax}}} | {{1,-{verMax}}} |", id, version);
            }
        }

        public override void Load() {
            StringBuilder superPlugins = new();
            PrintTable(superPlugins, SuperPlugins);
            StringBuilder plugins = new();
            PrintTable(plugins, Plugins);
            Trace.WriteLine(superPlugins);
            Trace.WriteLine(plugins);
            File.WriteAllText($"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/PluginReport.md",
$@"# SpacePi Dashboard Plugin Data

## Super Plugins

{superPlugins}

## Plugins

{plugins}
");
        }
    }
}
