using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.Generated;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.WPF {
    static class Entry {
        public static void Main() {
            IPluginFactory.Instance = new PluginFactoryImpl();
            foreach (IPlugin plugin in IPluginFactory.Instance.Plugins) {
                plugin.Load();
            }
        }
    }
}
