using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    public abstract class PipelineNode<TIn, TOut> : GenericPipeline {
        public abstract IEnumerable<TOut> ProcessMany(TIn @in);

        public override IEnumerable<object> ProcessGeneric(object @in) => ProcessMany((TIn) @in).Cast<object>();

        public virtual IEnumerable<TOut> Finish() {
            return Enumerable.Empty<TOut>();
        }

        public override IEnumerable<object> FinishGeneric() => Finish().Cast<object>();
    }
}
