using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    abstract class GeneratorPipeline : PipelineNode<PluginClass, SourceFile> {
        private SourceFile File;
        private bool HasPlugins;

        protected abstract string FileBaseName { get; }

        protected abstract string Header { get; }

        protected abstract string Footer { get; }

        protected abstract void Process(PluginClass plugin, StringBuilder src);

        public override void Init(GeneratorExecutionContext ctx) {
            File = new() {
                FileBaseName = FileBaseName
            };
            File.Contents.Append(Header);
            HasPlugins = false;
        }

        public override IEnumerable<SourceFile> Process(PluginClass plugin) {
            Process(plugin, File.Contents);
            HasPlugins = true;
            return Enumerable.Empty<SourceFile>();
        }

        public override IEnumerable<SourceFile> Finish() {
            if (HasPlugins) {
                File.Contents.Append(Footer);
                yield return File;
            }
        }
    }
}
