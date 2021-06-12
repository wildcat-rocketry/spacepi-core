using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    abstract class PluginPipelineHelper : SingularPipeline<PluginClass, PluginClass> {
        protected abstract new void _Process(PluginClass plugin);

        public override PluginClass Process(PluginClass plugin) {
            _Process(plugin);
            return plugin;
        }
    }

    abstract class PluginPipeline : PluginPipelineHelper {
        public abstract new void Process(PluginClass plugin);

        protected override void _Process(PluginClass plugin) => Process(plugin);
    }
}
