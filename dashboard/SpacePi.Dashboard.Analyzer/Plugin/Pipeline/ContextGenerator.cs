using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class ContextGenerator : GeneratorPipeline {
        protected override string FileBaseName => "ContextImpl";

        protected override string Header =>
@"using System;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Analyzer.Generated {
    public class ContextImpl : ContextBase {
        protected override IPluginInstance[] PluginsArray { get; } = new IPluginInstance[] {";

        protected override string Footer => @"
        };
    }
}
";

        protected override void Process(PluginClass plugin, StringBuilder src) {
            foreach (PluginInstance instance in plugin.Instance) {
                src.Append($@"
            ((IPlugin<{instance.FQCN}>) IPluginFactory.Instance.Plugins[{instance.Parent.Index}]).CreateInstance(),");
            }
        }
    }
}
