using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Pipeline {
    public abstract class BufferedPipeline<Type> : PipelineNode<Type, Type> {
        private readonly List<Type> List = new();

        public abstract void Process(Type @in);

        public override void Init(GeneratorExecutionContext ctx) {
            List.Clear();
        }

        public override IEnumerable<Type> ProcessMany(Type @in) {
            List.Add(@in);
            Process(@in);
            return Enumerable.Empty<Type>();
        }

        public override IEnumerable<Type> Finish() {
            return List;
        }
    }
}
