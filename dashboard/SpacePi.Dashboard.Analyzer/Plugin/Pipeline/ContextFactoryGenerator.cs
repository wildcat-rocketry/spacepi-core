using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class ContextFactoryGenerator : GeneratorPipeline {
        protected override string FileBaseName => "ContextFactoryImpl";

        protected override string Header =>
@"using System;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Analyzer.Generated {
    public class ContextFactoryImpl : IContextFactory {
        public IContext[] Contexts { get; }

        public void Dispose() {
            foreach (IContext context in Contexts) {
                context.Dispose();
            }
            GC.SuppressFinalize(this);
        }

        public ContextFactoryImpl(PluginFactoryImpl factory) {
            Contexts = new IContext[] {";

        protected override string Footer => @"
            };
        }
    }
}
";

        protected override void Process(PluginClass plugin, StringBuilder src) {
            foreach (ContextClass instance in plugin.Instance) {
                src.Append($@"
            ((IPlugin<{instance.FQCN}>) factory.Plugins[{instance.Parent.Index}]).CreateInstance(),");
            }
        }
    }
}
