﻿using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    public class PluginFactoryBindingGenerator : GeneratorPipeline {
        protected override string FileBaseName => "PluginFactoryImpl.Binding";

        protected override string Header =>
@$"using System;

namespace {Types.Generated} {{
    public partial class {Types.PluginFactoryImpl} {{
        public PluginFactoryImpl() {{";

        protected override string Footer => @"
        }
    }
}
";

        protected override void Process(PluginClass plugin, StringBuilder src) {
            foreach (BoundPlugin<PluginClass, PluginClass> binding in plugin.BoundPlugins) {
                src.Append($@"
            (({binding.Parent.FQCN}) Plugins[{binding.Parent.Index}]).{binding.FieldName} = ({binding.TargetClass.FQCN}) Plugins[{binding.TargetClass.Index}];");
            }
        }
    }
}
