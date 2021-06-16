using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.Generated;
using SpacePi.Dashboard.API.Startup;

namespace SpacePi.Dashboard.WPF {
    static class Entry {
        public static void Main() {
            StartupPlugin.Start(new PluginFactoryImpl());
        }
    }
}
