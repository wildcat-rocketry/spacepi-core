﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public class BindingGeneratorStream : GeneratorStream {
        public void AppendFactoryInstance(BindingFactory factory) =>
            Append($"F{factory.GeneratedClass.ClassName}");

        public void AppendUnboxing(BindingFactory factory, FactoryObject obj) =>
            AppendCast(
                () => AppendMethodCall(
                    () => AppendFactoryInstance(factory),
                    factory.Boxer.Unboxer,
                    () => AppendArrayIndex(
                        () => AppendProperty(
                            () => AppendFactoryInstance(factory),
                            factory.Array.Property),
                        obj.Index)),
                obj.ObjectType);

        public void AppendFactoryImpl(Context ctx, BindingFactory factory) =>
            AppendNamespace(
                GeneratedClass.Namespace,
                () => AppendClass(
                    factory.GeneratedClass.ClassName,
                    () => {
                        if (factory.GenerateEntryPoint) {
                            AppendMethod(
                                "public static",
                                "void",
                                "Main",
                                () => AppendStatement(
                                    () => AppendMethodCall(
                                        () => AppendConstructorCall(
                                            factory.GeneratedClass.ClassName,
                                            false),
                                        "Create")));
                        }
                        AppendMethod(
                            "public",
                            factory.Symbol,
                            "Create",
                            () => {
                                AppendAssignment(
                                    () => AppendNewVariable(
                                        factory.Symbol,
                                        () => AppendFactoryInstance(factory)),
                                    () => AppendConstructorCall(
                                        (string) null,
                                        false));
                                AppendAssignment(
                                    () => AppendProperty(
                                        () => AppendFactoryInstance(factory),
                                        factory.Array.Property),
                                    () => AppendNewArray(
                                        ((IArrayTypeSymbol) factory.Array.Property.Type).ElementType,
                                        factory.Objects.Select<FactoryObject, Action>(
                                            o => () => AppendMethodCall(
                                                () => AppendFactoryInstance(factory),
                                                factory.Boxer.Method,
                                                factory.Boxer.Parameters.Select<Parameter, Action>(p => p.Type switch {
                                                    Parameter.Types.FactoryObject => () => AppendConstructorCall(o.ObjectType, false),
                                                    Parameter.Types.ID => () => Append($"\"{o.Id}\""),
                                                    Parameter.Types.Parameter => () => Append(o.Parameters[p.Name].ToCSharpString()),
                                                    Parameter.Types.Priority => () => Append(o.Priority.ToString()),
                                                    _ => throw new Exception("Missing case statement")
                                                })))));
                                foreach (FactoryObject obj in factory.Objects) {
                                    foreach (Binding binding in obj.Bindings) {
                                        AppendAssignment(
                                            () => AppendProperty(
                                                () => AppendUnboxing(factory, obj),
                                                binding.Symbol),
                                            binding.Mode switch {
                                                Binding.Modes.Single => () => AppendUnboxing(
                                                    binding.Factory,
                                                    binding.Objects[0]),
                                                Binding.Modes.List => () => AppendNewArray(
                                                    ((INamedTypeSymbol) binding.Symbol.Type).TypeArguments[0],
                                                    binding.Objects.Select<FactoryObject, Action>(
                                                        o => () => AppendUnboxing(
                                                            binding.Factory,
                                                            o))),
                                                Binding.Modes.Dictionary => () => AppendConstructorCall(
                                                    ctx.StaticDictionary_2.Construct(
                                                        ((INamedTypeSymbol) binding.Symbol.Type).TypeArguments[0],
                                                        ((INamedTypeSymbol) binding.Symbol.Type).TypeArguments[1]),
                                                    true,
                                                    new Action[] {
                                                        () => AppendConstructorCall(binding.KeyType switch {
                                                            Binding.DictionaryKeyType.Int => ctx.PrimitiveEqualityComparers_Int,
                                                            Binding.DictionaryKeyType.String => ctx.PrimitiveEqualityComparers_String,
                                                            _ => throw new Exception("Missing case statement")
                                                        }, false)
                                                    }.Concat(binding.ObjectDict.GenerateTable().Select<(int, TypedConstant, FactoryObject), Action>(
                                                        t => () => AppendTuple(
                                                            false,
                                                            () => Append($"{t.Item1}"),
                                                            () => Append(t.Item2.ToCSharpString()),
                                                            () => AppendUnboxing(
                                                                binding.Factory,
                                                                t.Item3))))),
                                                Binding.Modes.Factory => () => AppendFactoryInstance(factory),
                                                _ => throw new Exception("Missing case statement")
                                            });
                                    }
                                }
                                foreach (IMethodSymbol loader in factory.LoadMethods) {
                                    AppendStatement(
                                        () => AppendMethodCall(
                                            () => AppendFactoryInstance(factory),
                                            loader));
                                }
                                AppendReturn(
                                    () => AppendFactoryInstance(factory));
                            });
                    },
                    ctx.IBoundFactory_1.Construct(factory.Symbol)));
    }
}
