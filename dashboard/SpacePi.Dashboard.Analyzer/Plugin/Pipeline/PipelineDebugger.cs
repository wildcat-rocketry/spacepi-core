using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.Analyzer.Pipeline;
using SpacePi.Dashboard.Analyzer.Plugin.Model;

namespace SpacePi.Dashboard.Analyzer.Plugin.Pipeline {
    class PipelineDebugger<Type> : SingularPipeline<Type> where Type : DebuggableRecord {
        private readonly string Prefix;
        private readonly string Suffix;
        private readonly bool AllowSpace;

        public override void Process(Type plugin) {
            StringBuilder str = new();
            str.Append(Prefix);
            if (plugin.Print(str) && AllowSpace) {
                str.Append(" ");
            }
            str.Append(Suffix);
            Console.Write(str);
        }

        public PipelineDebugger() {
            Prefix = "";
            Suffix = "\n";
            AllowSpace = false;
        }

        public PipelineDebugger(string name) {
            Prefix = $"{{ \"{name}\": ";
            Suffix = "}\n";
            AllowSpace = true;
        }
    }
}
