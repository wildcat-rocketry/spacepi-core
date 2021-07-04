using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public record PluginFactoryClass : PluginClass {
        public bool GenerateMain { get; set; }

        public PluginFactoryClass() => Priority = int.MinValue;
    }
}
