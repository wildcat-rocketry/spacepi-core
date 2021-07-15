using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer {
    public class Context {
        public readonly Compilation Compilation;
        public readonly Diagnostics Diagnostics;

        public IEnumerable<INamedTypeSymbol> Types {
            get {
                Stack<INamespaceSymbol> stack = new();
                stack.Push(Compilation.GlobalNamespace);
                while (stack.Count > 0) {
                    INamespaceSymbol ns = stack.Pop();
                    foreach (INamespaceSymbol sub in ns.GetNamespaceMembers()) {
                        stack.Push(sub);
                    }
                    foreach (INamedTypeSymbol type in ns.GetTypeMembers()) {
                        yield return type;
                    }
                }
            }
        }

        public INamedTypeSymbol GetType<Type>() {
            string fqcn = typeof(Type).FullName.Split(new[] { '[' }, 2)[0];
            INamedTypeSymbol sym = Compilation.GetTypeByMetadataName(fqcn);
            if (sym == null) {
                Diagnostics.APITypeMissing.Report(fqcn);
            }
            return sym;
        }

        public readonly INamedTypeSymbol Attribute;
        public readonly INamedTypeSymbol AttributeUsageAttribute;
        public readonly INamedTypeSymbol BindingFactoryAttribute;
        public readonly INamedTypeSymbol BindingFactoryArrayAttribute;
        public readonly INamedTypeSymbol BindingFactoryBoxerAttribute;
        public readonly INamedTypeSymbol BindingFactoryLoaderAttribute;
        public readonly INamedTypeSymbol BindingFactoryUnboxerAttribute;
        public readonly INamedTypeSymbol BindingIDAttribute;
        public readonly INamedTypeSymbol BindingFactoryObjectAttribute;
        public readonly INamedTypeSymbol BindingParameterAttribute;
        public readonly INamedTypeSymbol BindingPriorityAttribute;
        public readonly INamedTypeSymbol EntryPointAttribute;
        public readonly INamedTypeSymbol IBoundFactory_1;
        public readonly INamedTypeSymbol IEnumerable_1;
        public readonly INamedTypeSymbol GenerateBindingsAttribute;

        public Context(Compilation compilation, Diagnostics diags) {
            Compilation = compilation;
            Diagnostics = diags;
            Attribute                          = GetType<Attribute                     >();
            AttributeUsageAttribute            = GetType<AttributeUsageAttribute       >();
            BindingFactoryAttribute            = GetType<BindingFactoryAttribute       >();
            BindingFactoryArrayAttribute       = GetType<BindingFactoryArrayAttribute  >();
            BindingFactoryBoxerAttribute       = GetType<BindingFactoryBoxerAttribute  >();
            BindingFactoryLoaderAttribute      = GetType<BindingFactoryLoaderAttribute >();
            BindingFactoryUnboxerAttribute     = GetType<BindingFactoryUnboxerAttribute>();
            BindingIDAttribute                 = GetType<BindingIDAttribute            >();
            BindingFactoryObjectAttribute      = GetType<BindingFactoryObjectAttribute >();
            BindingParameterAttribute          = GetType<BindingParameterAttribute     >();
            BindingPriorityAttribute           = GetType<BindingPriorityAttribute      >();
            EntryPointAttribute                = GetType<EntryPointAttribute           >();
            IBoundFactory_1                    = GetType<IBoundFactory<object>         >();
            IEnumerable_1                      = GetType<IEnumerable<object>           >();
            GenerateBindingsAttribute          = GetType<GenerateBindingsAttribute     >();
        }
    }
}
