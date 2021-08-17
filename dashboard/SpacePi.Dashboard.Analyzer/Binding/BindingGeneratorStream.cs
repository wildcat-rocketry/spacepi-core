using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// A stream to generate the binding source code
    /// </summary>
    public class BindingGeneratorStream : GeneratorStream {
        /// <summary>
        /// Appends the name of the property used to store the factory instance
        /// </summary>
        /// <param name="factory">The factory instance</param>
        public void AppendFactoryInstance(BindingFactoryInstance factory) =>
            Append($"F{factory.GeneratedClass.ClassName}");

        /// <summary>
        /// Appends the unboxing of a factory object
        /// </summary>
        /// <param name="factory">The factory instance</param>
        /// <param name="obj">The object to unbox</param>
        public void AppendUnboxing(BindingFactoryInstance factory, FactoryObjectInstance obj) =>
            AppendCast(
                () => AppendMethodCall(
                    () => AppendFactoryInstance(factory),
                    factory.BindingFactory.Boxer.Unboxer,
                    () => AppendArrayIndex(
                        () => AppendProperty(
                            () => AppendFactoryInstance(factory),
                            factory.BindingFactory.Array.Property),
                        obj.FactoryObject.Index)),
                obj.FactoryObject.ObjectType);

        /// <summary>
        /// Appends the implementation of a factory instance
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="factory">The factory instance</param>
        public void AppendFactoryImpl(Context ctx, BindingFactoryInstance factory) =>
            AppendNamespace(
                GeneratedClass.Namespace,
                () => AppendClass(
                    factory.GeneratedClass.ClassName,
                    () => {
                        foreach (BindingFactoryInstance parent in factory.Parents) {
                            AppendPropertyDef(
                                "public",
                                parent.BindingFactory.Symbol,
                                () => AppendFactoryInstance(parent));
                        }
                        AppendMethod(
                            "public",
                            factory.BindingFactory.Symbol,
                            "Create",
                            () => {
                                AppendAssignment(
                                    () => AppendNewVariable(
                                        factory.BindingFactory.Symbol,
                                        () => AppendFactoryInstance(factory)),
                                    () => AppendConstructorCall(
                                        (string) null,
                                        false));
                                foreach ((BindingFactoryInstance subfactory, IPropertySymbol prop) in factory.Subfactories.Zip(factory.BindingFactory.Subfactories, (a, b) => (a, b))) {
                                    AppendAssignment(
                                        () => AppendProperty(
                                            () => AppendFactoryInstance(factory),
                                            prop),
                                        () => AppendConstructorCall(
                                            subfactory.GeneratedClass.ClassName,
                                            false,
                                            null,
                                            subfactory.Parents.Select<BindingFactoryInstance, Action>(
                                                p => () => AppendAssignmentExpression(
                                                    () => AppendFactoryInstance(p),
                                                    () => AppendFactoryInstance(p)))));
                                }
                                AppendAssignment(
                                    () => AppendProperty(
                                        () => AppendFactoryInstance(factory),
                                        factory.BindingFactory.Array.Property),
                                    () => AppendNewArray(
                                        ((IArrayTypeSymbol) factory.BindingFactory.Array.Property.Type).ElementType,
                                        factory.BindingFactory.Objects.Select<FactoryObject, Action>(
                                            o => () => AppendMethodCall(
                                                () => AppendFactoryInstance(factory),
                                                factory.BindingFactory.Boxer.Method,
                                                factory.BindingFactory.Boxer.Parameters.Select<Parameter, Action>(p => p.Type switch {
                                                    Parameter.Types.FactoryObject => () => AppendConstructorCall(o.ObjectType, false),
                                                    Parameter.Types.ID => () => Append($"\"{o.Id}\""),
                                                    Parameter.Types.Parameter => () => Append(o.Parameters[p.Name].ToCSharpString()),
                                                    Parameter.Types.Priority => () => Append(o.Priority.ToString()),
                                                    _ => throw new Exception("Missing case statement")
                                                })))));
                                foreach (FactoryObjectInstance obj in factory.Objects.Values) {
                                    foreach (BindingInstance binding in obj.Bindings) {
                                        AppendAssignment(
                                            () => AppendProperty(
                                                () => AppendUnboxing(factory, obj),
                                                binding.Binding.Symbol),
                                            binding.Binding.Mode switch {
                                                Binding.Modes.Single => () => AppendUnboxing(
                                                    binding.Factory,
                                                    binding.Objects[0]),
                                                Binding.Modes.List => () => AppendNewArray(
                                                    ((INamedTypeSymbol) binding.Binding.Symbol.Type).TypeArguments[0],
                                                    binding.Objects.Select<FactoryObjectInstance, Action>(
                                                        o => () => AppendUnboxing(
                                                            binding.Factory,
                                                            o))),
                                                Binding.Modes.Dictionary => () => AppendConstructorCall(
                                                    ctx.StaticDictionary_2.Construct(
                                                        ((INamedTypeSymbol) binding.Binding.Symbol.Type).TypeArguments[0],
                                                        ((INamedTypeSymbol) binding.Binding.Symbol.Type).TypeArguments[1]),
                                                    true,
                                                    new Action[] {
                                                        () => AppendConstructorCall(binding.Binding.KeyType switch {
                                                            Binding.DictionaryKeyType.Int => ctx.PrimitiveEqualityComparers_Int,
                                                            Binding.DictionaryKeyType.String => ctx.PrimitiveEqualityComparers_String,
                                                            _ => throw new Exception("Missing case statement")
                                                        }, false)
                                                    }.Concat(binding.ObjectDict.GenerateTable().Select<(int, TypedConstant, FactoryObjectInstance), Action>(
                                                        t => () => AppendTuple(
                                                            false,
                                                            () => Append($"{t.Item1}"),
                                                            () => Append(t.Item2.ToCSharpString()),
                                                            () => AppendUnboxing(
                                                                binding.Factory,
                                                                t.Item3)))),
                                                    null),
                                                Binding.Modes.Factory => () => AppendFactoryInstance(binding.Factory),
                                                _ => throw new Exception("Missing case statement")
                                            });
                                    }
                                }
                                foreach (IMethodSymbol loader in factory.BindingFactory.LoadMethods) {
                                    AppendStatement(
                                        () => AppendMethodCall(
                                            () => AppendFactoryInstance(factory),
                                            loader));
                                }
                                AppendReturn(
                                    () => AppendFactoryInstance(factory));
                            });
                    },
                    ctx.IBoundFactory_1.Construct(factory.BindingFactory.Symbol)));

        /// <summary>
        /// Appends the implementation of the main class
        /// </summary>
        /// <param name="mainClass">The main class to generate</param>
        /// <param name="roots">The list of root nodes to generate in the main function</param>
        public void AppendMainClass(GeneratedClass mainClass, IEnumerable<BindingFactoryInstance> roots) =>
            AppendNamespace(
                GeneratedClass.Namespace,
                () => AppendClass(
                    mainClass.ClassName,
                    () => AppendMethod(
                        "public static",
                        "void",
                        "Main",
                        () => {
                            foreach (BindingFactoryInstance root in roots) {
                                AppendStatement(
                                    () => AppendMethodCall(
                                        () => AppendConstructorCall(
                                            root.GeneratedClass.ClassName,
                                            false),
                                        "Create"));
                            }
                        })));
    }
}
