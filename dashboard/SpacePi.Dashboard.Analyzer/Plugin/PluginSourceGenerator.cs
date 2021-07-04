using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    [Generator]
    public class PluginSourceGenerator : SourceGenerator {
        private void GenerateBindings<TTarget, TBindingTarget>(StringBuilder str, IEnumerable<TTarget> targets, Func<TTarget, IEnumerable<Binding<TBindingTarget>>> bindingsFunc) where TTarget : IClassBindingTarget where TBindingTarget : IEquatable<TBindingTarget>, IClassBindingTarget {
            string targetArray;
            if (targets is IEnumerable<PluginClass>) {
                targetArray = "Plugins";
            } else if (targets is IEnumerable<ContextClass>) {
                targetArray = "contexts";
            } else {
                str.Append(@"
            // unknown binding target");
                return;
            }
            string boundArray = null;
            foreach (TTarget cls in targets) {
                foreach (Binding<TBindingTarget> binding in bindingsFunc(cls)) {
                    if (boundArray == null) {
                        if (binding is Binding<PluginClass>) {
                            boundArray = "Plugins";
                        } else if (binding is Binding<ContextClass>) {
                            boundArray = "contexts";
                        } else {
                            str.Append(@"
            // unknown binding source");
                            return;
                        }
                    }
                    str.Append($@"
            (({binding.Property.ContainingType}) {targetArray}[{cls.Index}]).{binding.Property.Name} = ");
                    
                    if (binding is IClassBinding clsBinding) {
                        str.Append($"({binding.Property.Type}) {boundArray}[{clsBinding.Target.Index}]; // {clsBinding.Target.Symbol}");
                    } else if (binding is ArrayBinding<TBindingTarget> arrayBinding) {
                        ITypeSymbol iface = ArrayBinding<TBindingTarget>.GetInterfaceType(binding.Property);
                        str.Append($"new {iface}[] {{");
                        foreach (TBindingTarget target in arrayBinding.Targets) {
                            str.Append($@"
                ({iface}) {boundArray}[{target.Index,6}], // {target.Symbol}");
                        }
                        str.Append(@"
            };");
                    } else {
                        str.Append("null;");
                    }
                }
            }
        }

        public void GenerateSource(StringBuilder str, PluginFactoryClass factory, PluginRegistry plugins, ContextRegistry contexts) {
            Version ver = Assembly.GetExecutingAssembly().GetName().Version;
            str.Append(
$@"namespace {factory.Symbol.ContainingNamespace} {{
    public partial class {factory.Symbol.Name} : {Types.IPluginFactory} {{
        private readonly {Types.IPlugin}[] Plugins;
        private bool Disposed = false;

        string {Types.IPlugin}.Name => ""{factory.Symbol}"";

        {Types.Version} {Types.IPlugin}.Version => new({ver.Major}, {ver.Minor}, {ver.Build}, {ver.Revision});");
            if (factory.GenerateMain) {
                str.Append($@"

        public static void Main() => new {factory.Symbol}();");
            }
            str.Append($@"

        void {Types.IPluginFactory}.CreateContext() {{
            {Types.IContext}[] contexts = new {Types.IContext}[] {{");
            foreach (ContextClass cls in contexts.Contexts.OrderBy(c => c.Index)) {
                str.Append($@"
                (({cls.PluginInterface}) Plugins[{cls.OwnerPlugin.Index}]).CreateContext(), // {cls.OwnerPlugin.Symbol}");
            }
            str.Append(@"
            };");
            GenerateBindings(str, contexts.Contexts, c => c.PluginBindings);
            GenerateBindings(str, contexts.Contexts, c => c.ContextBindings);
            str.Append($@"
            foreach ({Types.IContext} context in contexts) {{
                context.Load();
            }}
        }}

        void {Types.IDisposable}.Dispose() {{
            if (!Disposed) {{
                Disposed = true;
                foreach ({Types.IPlugin} plugin in Plugins) {{
                    plugin.Dispose();
                }}
            }}
        }}

        public {factory.Symbol.Name}() {{
            Plugins = new {Types.IPlugin}[] {{");
            foreach (PluginClass cls in plugins.Plugins.OrderBy(p => p.Index)) {
                if (cls == factory) {
                    str.Append(@"
                this,");
                } else {
                    str.Append($@"
                new {cls.Symbol}(),");
                }
            }
            str.Append(@"
            };");
            GenerateBindings(str, plugins.Plugins, p => p.PluginBindings);
            str.Append($@"
            foreach ({Types.IPlugin} plugin in Plugins) {{
                plugin.Load();
            }}
        }}
    }}
}}
");
        }

        protected override IEnumerable<IEnumerable<Diagnostic>> Generate(GeneratorExecutionContext ctx) {
            PluginAnalyzer analyzer = new(ctx.Compilation);
            yield return analyzer.AnalyzeAll();
            yield return analyzer.FindAllProbableContexts();
            yield return analyzer.BindAll();
            analyzer.PluginRegistry.GenerateIndices();
            analyzer.ContextRegistry.GenerateIndices();
            if (!HasErrored) {
                foreach (PluginFactoryClass factory in analyzer.PluginRegistry.Plugins.OfType<PluginFactoryClass>()) {
                    StringBuilder str = new();
                    GenerateSource(str, factory, analyzer.PluginRegistry, analyzer.ContextRegistry);
                    ctx.AddSource(factory.Symbol.Name, str.ToString());
                }
            }
        }
    }
}
