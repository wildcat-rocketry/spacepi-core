using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    public abstract class SingularPipeline<TIn, TOut> : PipelineNode<TIn, TOut> {
        public abstract TOut ProcessOne(TIn @in);

        public override IEnumerable<TOut> ProcessMany(TIn @in) {
            yield return ProcessOne(@in);
        }
    }

    public abstract class SingularPipeline<Type> : SingularPipeline<Type, Type> {
        public abstract void Process(Type @in);

        public override Type ProcessOne(Type @in) {
            Process(@in);
            return @in;
        }
    }
}
