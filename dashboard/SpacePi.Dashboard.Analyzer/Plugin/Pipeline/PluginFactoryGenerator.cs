using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PluginFactoryGenerator : GeneratorPipeline {
        protected override string FileBaseName => "PluginFactoryImpl";

        protected override string Header =>
@"using System;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Analyzer.Generated {
    public class PluginFactoryImpl : IPluginFactory {
        public IPlugin[] Plugins { get; } = new IPlugin[] {";

        protected override string Footer => @"
        };

        public IContext CreateContext() => new ContextImpl();
    }
}
";

        protected override void Process(PluginClass plugin, StringBuilder src) => src.Append($@"
            new {plugin.FQCN}(),");
    }
}
