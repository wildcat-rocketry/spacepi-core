using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    class PipelineUnion : GenericPipeline {
        private readonly GenericPipeline First;
        private readonly GenericPipeline Second;

        public override void Init(GeneratorExecutionContext ctx) {
            First.Init(ctx);
            Second.Init(ctx);
        }

        public override IEnumerable<object> ProcessGeneric(object @in) => First.ProcessGeneric(@in).SelectMany(i => Second.ProcessGeneric(i));

        public override IEnumerable<object> FinishGeneric() => Enumerable.Range(0, 2).SelectMany(i => i == 0 ? First.FinishGeneric().SelectMany(i => Second.ProcessGeneric(i)) : Second.FinishGeneric());

        public PipelineUnion(GenericPipeline first, GenericPipeline second) {
            First = first;
            Second = second;
        }
    }
}
