using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class FactoryGenerator : PipelineNode<PluginClass, SourceFile> {
        private SourceFile File;
        private bool AnyPlugins;

        public override void Init(GeneratorExecutionContext ctx) {
            File = new() {
                FileBaseName = "PluginContext"
            };
            File.Contents.Append(new string(@"
using System;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Analyzer {
    public static class PluginContext {
        public static void Setup() {
            PluginFactory.Initialize(new IPlugin[] {".SkipWhile(c => c != 'u').ToArray()));
            AnyPlugins = false;
        }

        public override IEnumerable<SourceFile> Process(PluginClass plugin) {
            File.Contents.Append($@"
                new {plugin.FQCN}(),");
            AnyPlugins = true;
            return Enumerable.Empty<SourceFile>();
        }

        public override IEnumerable<SourceFile> Finish() {
            if (AnyPlugins) {
                File.Contents.Append(@"
            });
        }
    }
}
");
                yield return File;
            }
        }
    }
}
