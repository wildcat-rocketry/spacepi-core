using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    class PipelineTee : GenericPipeline {
        private readonly GenericPipeline[] Pipes;

        public override void Init(GeneratorExecutionContext ctx) {
            foreach (GenericPipeline pipe in Pipes) {
                pipe.Init(ctx);
            }
        }

        public override IEnumerable<object> ProcessGeneric(object @in) => Pipes.SelectMany(p => p.ProcessGeneric(@in));

        public override IEnumerable<object> FinishGeneric() => Pipes.SelectMany(p => p.FinishGeneric());

        public PipelineTee(params GenericPipeline[] pipes) => Pipes = pipes;
    }
}
