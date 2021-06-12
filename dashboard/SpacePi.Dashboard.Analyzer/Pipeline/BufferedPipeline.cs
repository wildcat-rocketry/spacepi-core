using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    abstract class BufferedPipelineHelper<Type> : PipelineNode<Type, Type> {
        private readonly List<Type> List = new();

        protected abstract void _Process(Type @in);

        public override void Init(GeneratorExecutionContext ctx) {
            List.Clear();
        }

        public override IEnumerable<Type> Process(Type @in) {
            List.Add(@in);
            _Process(@in);
            return Enumerable.Empty<Type>();
        }

        public override IEnumerable<Type> Finish() {
            return List;
        }
    }

    abstract class BufferedPipeline<Type> : BufferedPipelineHelper<Type> {
        public abstract new void Process(Type @in);

        protected override void _Process(Type @in) => Process(@in);
    }
}
