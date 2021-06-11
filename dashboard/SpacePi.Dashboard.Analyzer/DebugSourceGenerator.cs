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
            TextWriter stdout = Console.Out;
            TextWriter stderr = Console.Error;
            Directory.CreateDirectory(BaseDir);
            using (TextWriter log = new StreamWriter(new FileStream(Path.Combine(BaseDir, $"{GetType().Name}.{name}.log"), FileMode.Create))) {
                Console.SetOut(log);
                try {
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
            }
        }
    }
}
