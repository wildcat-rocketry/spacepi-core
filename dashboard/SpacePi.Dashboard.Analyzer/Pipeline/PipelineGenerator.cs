using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    public static class PipelineGenerator {
        public static IEnumerable<object> RunPipelineGeneric(GeneratorExecutionContext ctx, GenericPipeline pipeline) {
            pipeline.Init(ctx);
            return Enumerable.Range(0, 2).SelectMany(i => i == 0 ? pipeline.ProcessGeneric(new Null()) : pipeline.FinishGeneric());
        }

        public static IEnumerable<TOut> RunPipeline<TOut>(GeneratorExecutionContext ctx, GenericPipeline pipeline) => RunPipelineGeneric(ctx, pipeline).Cast<TOut>();

        public static void RunSourcePipeline(GeneratorExecutionContext ctx, GenericPipeline pipeline) {
            foreach (SourceFile src in RunPipeline<SourceFile>(ctx, pipeline)) {
                ctx.AddSource(src.FileBaseName, src.Contents.ToString());
            }
        }
    }
}
