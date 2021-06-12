using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class DebugSourceGenerator : ISourceGenerator {
        private static readonly string BaseDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.Analyzer/{nameof(DebugSourceGenerator)}";

        public abstract void DebugExecute(GeneratorExecutionContext ctx);

        public virtual void DebugInitialize(GeneratorInitializationContext ctx) {
        }

        public virtual void Execute(GeneratorExecutionContext ctx) => DebugAction(nameof(Execute), () => DebugExecute(ctx));

        public void Initialize(GeneratorInitializationContext ctx) => DebugAction(nameof(Initialize), () => DebugInitialize(ctx));

        protected void DebugAction(string name, Action action) {
            using StringWriter log = new();
            try {
                TextWriter stdout = Console.Out;
                Console.SetOut(log);
                try {
                    TextWriter stderr = Console.Error;
                    Console.SetError(log);
                    try {
                        action();
                    } catch (Exception ex) {
                        Console.Error.WriteLine(ex);
                    } finally {
                        Console.SetError(stderr);
                    }
                } finally {
                    Console.SetOut(stdout);
                }
            } finally {
                string str = log.ToString();
                if (str.Length > 0) {
                    Directory.CreateDirectory(BaseDir);
                    File.WriteAllText(Path.Combine(BaseDir, $"{GetType().Name}.{name}.log"), str);
                }
            }
        }
    }
}
