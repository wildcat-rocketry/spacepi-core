using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    abstract class GenericPipeline {
        public virtual void Init(GeneratorExecutionContext ctx) {
        }

        public abstract IEnumerable<object> ProcessGeneric(object @in);

        public virtual IEnumerable<object> FinishGeneric() => Enumerable.Empty<object>();

        public static GenericPipeline operator +(GenericPipeline lhs, GenericPipeline rhs) => new PipelineUnion(lhs, rhs);

        public static GenericPipeline operator |(GenericPipeline lhs, GenericPipeline rhs) => new PipelineTee(lhs, rhs);
    }
}
