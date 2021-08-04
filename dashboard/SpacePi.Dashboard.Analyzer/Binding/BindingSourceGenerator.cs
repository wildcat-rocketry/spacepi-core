//#define BINDING_SOURCE_GEN_DEBUG

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// The source generator which handles bindings
    /// </summary>
    [Generator]
    public class BindingSourceGenerator : SourceGenerator {
        /// <summary>
        /// Generates a source to add to the compilation
        /// </summary>
        /// <param name="generator">The generator context</param>
        /// <param name="cls">The class to generate</param>
        /// <param name="code">The code to put in the class</param>
        private void GenerateSource(GeneratorExecutionContext generator, GeneratedClass cls, Action<BindingGeneratorStream> code) {
            BindingGeneratorStream stream = new();
            code(stream);
            generator.AddSource(cls.ClassName, stream.ToString());
#if BINDING_SOURCE_GEN_DEBUG
                Console.WriteLine($@"
// {cls.ClassName}.cs
{stream}
");
#endif
        }

        /// <summary>
        /// Generates the bindings
        /// </summary>
        /// <param name="generator">The generator context</param>
        /// <param name="ctx">The compilation context</param>
        protected override void Generate(GeneratorExecutionContext generator, Context ctx) {
            BindingContext bindings = new(ctx);
            if (!ctx.Compilation.Assembly.BeginValidation().HasAttribute(ctx.GenerateBindingsAttribute).Check()) {
                return;
            }
            if (ctx.Diagnostics.Any(d => d.Severity == DiagnosticSeverity.Error)) {
                return;
            }
            GenerateSource(generator, bindings.MainClass, s => s.AppendMainClass(bindings.MainClass, bindings.Roots));
            foreach (BindingFactoryInstance f in bindings.AllFactories) {
                GenerateSource(generator, f.GeneratedClass, s => s.AppendFactoryImpl(ctx, f));
            }
        }
    }
}
