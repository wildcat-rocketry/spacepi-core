using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// A context representing the current compilation
    /// </summary>
    public class Context {
        /// <summary>
        /// The roslyn compilation context
        /// </summary>
        public readonly Compilation Compilation;
        /// <summary>
        /// The currently reported diagnostics
        /// </summary>
        public readonly Diagnostics Diagnostics;

        /// <summary>
        /// Gets all the types declared in the compilation context
        /// </summary>
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

        /// <summary>
        /// Looks up a type in the compilation context
        /// </summary>
        /// <typeparam name="Type">The type to look up</typeparam>
        /// <returns>The equivalent symbol in the compilation context</returns>
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
        public readonly INamedTypeSymbol BindingDictionaryKeyAttribute;
        public readonly INamedTypeSymbol BindingFactoryAttribute;
        public readonly INamedTypeSymbol BindingFactoryArrayAttribute;
        public readonly INamedTypeSymbol BindingFactoryBoxerAttribute;
        public readonly INamedTypeSymbol BindingFactoryLoaderAttribute;
        public readonly INamedTypeSymbol BindingFactoryUnboxerAttribute;
        public readonly INamedTypeSymbol BindingIDAttribute;
        public readonly INamedTypeSymbol BindingMatchIDAttribute;
        public readonly INamedTypeSymbol BindingFactoryObjectAttribute;
        public readonly INamedTypeSymbol BindingParameterAttribute;
        public readonly INamedTypeSymbol BindingPriorityAttribute;
        public readonly INamedTypeSymbol EntryPointAttribute;
        public readonly INamedTypeSymbol IBoundFactory_1;
        public readonly INamedTypeSymbol IDictionary_2;
        public readonly INamedTypeSymbol IEnumerable_1;
        public readonly INamedTypeSymbol GenerateBindingsAttribute;
        public readonly INamedTypeSymbol Object;
        public readonly INamedTypeSymbol PrimitiveEqualityComparers_Int;
        public readonly INamedTypeSymbol PrimitiveEqualityComparers_String;
        public readonly INamedTypeSymbol StaticDictionary_2;
        public readonly INamedTypeSymbol SubfactoryAttribute;

        /// <summary>
        /// Creates a new Context
        /// </summary>
        /// <param name="compilation">The roslyn compilation context</param>
        /// <param name="diags">The currently reported diagnostics</param>
        public Context(Compilation compilation, Diagnostics diags) {
            Compilation = compilation;
            Diagnostics = diags;
            Attribute                          = GetType<Attribute                        >();
            AttributeUsageAttribute            = GetType<AttributeUsageAttribute          >();
            BindingDictionaryKeyAttribute      = GetType<BindingDictionaryKeyAttribute    >();
            BindingFactoryAttribute            = GetType<BindingFactoryAttribute          >();
            BindingFactoryArrayAttribute       = GetType<BindingFactoryArrayAttribute     >();
            BindingFactoryBoxerAttribute       = GetType<BindingFactoryBoxerAttribute     >();
            BindingFactoryLoaderAttribute      = GetType<BindingFactoryLoaderAttribute    >();
            BindingFactoryUnboxerAttribute     = GetType<BindingFactoryUnboxerAttribute   >();
            BindingIDAttribute                 = GetType<BindingIDAttribute               >();
            BindingMatchIDAttribute            = GetType<BindingMatchIDAttribute          >();
            BindingFactoryObjectAttribute      = GetType<BindingFactoryObjectAttribute    >();
            BindingParameterAttribute          = GetType<BindingParameterAttribute        >();
            BindingPriorityAttribute           = GetType<BindingPriorityAttribute         >();
            EntryPointAttribute                = GetType<EntryPointAttribute              >();
            IBoundFactory_1                    = GetType<IBoundFactory<object>            >();
            IDictionary_2                      = GetType<IDictionary<object, object>      >();
            IEnumerable_1                      = GetType<IEnumerable<object>              >();
            GenerateBindingsAttribute          = GetType<GenerateBindingsAttribute        >();
            Object                             = GetType<object                           >();
            PrimitiveEqualityComparers_Int     = GetType<PrimitiveEqualityComparers.Int   >();
            PrimitiveEqualityComparers_String  = GetType<PrimitiveEqualityComparers.String>();
            StaticDictionary_2                 = GetType<StaticDictionary<object, object> >();
            SubfactoryAttribute                = GetType<SubfactoryAttribute              >();
        }
    }
}
