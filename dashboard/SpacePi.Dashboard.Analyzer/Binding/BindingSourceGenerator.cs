#define DEBUG

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    [Generator]
    public class BindingSourceGenerator : SourceGenerator {
        protected override void Generate(GeneratorExecutionContext generator, Context ctx) {
            BindingContext bindings = new(ctx);
            if (!ctx.Compilation.Assembly.BeginValidation().HasAttribute(ctx.GenerateBindingsAttribute).Check()) {
                return;
            }
            if (ctx.Diagnostics.Any(d => d.Severity == DiagnosticSeverity.Error)) {
                return;
            }
            BindingGeneratorStream stream = new();
            foreach (BindingFactory factory in bindings.Factories) {
                stream.Clear();
                stream.AppendFactoryImpl(ctx, factory);
                generator.AddSource(factory.GeneratedClass.ClassName, stream.ToString());
#if DEBUG
                Console.WriteLine($@"
// {factory.GeneratedClass.ClassName}.cs
{stream}
");
#endif
            }
        }
    }
}
