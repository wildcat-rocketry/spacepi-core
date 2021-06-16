using SpacePi.Dashboard.Analyzer.Plugin.Model;
using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class ContextFactoryBindingGenerator : GeneratorPipeline {
        protected override string FileBaseName => "ContextFactoryImpl.Binding";

        protected override string Header =>
@"using System;

namespace SpacePi.Dashboard.Analyzer.Generated {
    public partial class ContextFactoryImpl {
        private void Bind(PluginFactoryImpl factory) {";

        protected override string Footer => @"
        }
    }
}
";

        protected override void Process(PluginClass plugin, StringBuilder src) {
            foreach (ContextClass context in plugin.Contexts) {
                foreach (BoundPlugin<ContextClass, PluginClass> binding in context.BoundPlugins) {
                    src.Append($@"
            (({binding.Parent.FQCN}) Contexts[{binding.Parent.Index}]).{binding.FieldName} = ({binding.TargetClass.FQCN}) factory.Plugins[{binding.TargetClass.Index}];");
                }
                foreach (BoundPlugin<ContextClass, ContextClass> binding in context.BoundContexts) {
                    src.Append($@"
            (({binding.Parent.FQCN}) Contexts[{binding.Parent.Index}]).{binding.FieldName} = ({binding.TargetClass.FQCN}) Contexts[{binding.TargetClass.Index}];");
                }
            }
        }
    }
}
