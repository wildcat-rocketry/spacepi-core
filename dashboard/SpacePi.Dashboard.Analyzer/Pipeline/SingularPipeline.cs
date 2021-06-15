using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    abstract class SingularPipelineHelper<TIn, TOut> : PipelineNode<TIn, TOut> {
        protected abstract TOut _Process(TIn @in);

        public override IEnumerable<TOut> Process(TIn @in) {
            yield return _Process(@in);
        }
    }

    abstract class SingularPipeline<TIn, TOut> : SingularPipelineHelper<TIn, TOut> {
        public abstract new TOut Process(TIn @in);

        protected override TOut _Process(TIn @in) => Process(@in);
    }

    abstract class SingularPipeline<Type> : SingularPipelineHelper<Type, Type> {
        public abstract new void Process(Type @in);

        protected override Type _Process(Type @in) {
            Process(@in);
            return @in;
        }
    }
}
