using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public interface IPluginBindable {
        List<Binding<PluginClass>> PluginBindings { get; }
    }
}
