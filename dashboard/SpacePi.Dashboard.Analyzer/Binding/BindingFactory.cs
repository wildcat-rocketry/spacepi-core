using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record BindingFactory : IGeneratorSource {
        public INamedTypeSymbol Symbol;
        public INamedTypeSymbol DeclarativeAttribute;
        public INamedTypeSymbol BindingAttribute;
        public BindingFactoryArray Array = new();
        public BindingFactoryBoxer Boxer = new();
        public IMethodSymbol[] LoadMethods;
        public bool GenerateEntryPoint;
        public FactoryObject[] Objects;

        public GeneratedClass GeneratedClass { get; } = new();

        public static IEnumerable<BindingFactory> ParseAll(Context ctx) {
            BindingFactory obj = null;
            foreach ((INamedTypeSymbol symbol, (INamedTypeSymbol declarativeAttr, INamedTypeSymbol bindingAttr)) in ctx.Types
                .BeginValidation()
                .HasAttribute(ctx.BindingFactoryAttribute, a => ((INamedTypeSymbol) a.ConstructorArguments[0].Value, (INamedTypeSymbol) a.ConstructorArguments[1].Value))
                .WhereValid()) {
                if (obj == null) {
                    obj = new();
                }
                if (!symbol.BeginValidation()
                    .IsClass<INamedTypeSymbol>()
                    .IsNotGenericType<INamedTypeSymbol>()
                    .IsNotAbstractType<INamedTypeSymbol>()
                    .IsPublic()
                    .IsDefaultConstructible<INamedTypeSymbol>()
                    .Require(ctx.Diagnostics)) {
                    continue;
                }
                obj.Symbol = symbol;
                obj.DeclarativeAttribute = declarativeAttr;
                obj.BindingAttribute = bindingAttr;
                int start = ctx.Diagnostics.Count();
                new[] {
                        (declarativeAttr, AttributeTargets.Class),
                        (bindingAttr, AttributeTargets.Property)
                    }.BeginValidation()
                    .Extends(ctx.Attribute)
                    .HasAttribute(ctx.AttributeUsageAttribute, (a, t) => ((int) a.ConstructorArguments.FirstOrDefault().Value) == (int) t)
                    .ContextTrue("AttributeUsage incorrectly set on attribute")
                    .Require(ctx.Diagnostics);
                obj.Array.TryParse(symbol, ctx);
                obj.Boxer.TryParse(symbol, obj.Array, ctx);
                obj.LoadMethods = symbol.GetAllMembers()
                    .OfType<IMethodSymbol>()
                    .BeginValidation()
                    .HasAttribute(ctx.BindingFactoryLoaderAttribute)
                    .IsPublic()
                    .IsInstance()
                    .IsOrdinaryMethod()
                    .IsParameterless()
                    .WhereValid()
                    .StripContext()
                    .ToArray();
                obj.GenerateEntryPoint = symbol.BeginValidation()
                    .HasAttribute(ctx.EntryPointAttribute)
                    .Check();
                if (ctx.Diagnostics.Count() == start) {
                    yield return obj;
                    obj = null;
                }
            }
        }

        public void FindObjects(Context ctx) {
            List<FactoryObject> objs = new();
            FactoryObject obj = null;
            foreach ((INamedTypeSymbol symbol, AttributeData attr) in ctx.Types.BeginValidation()
                .HasAttribute(DeclarativeAttribute, a => a)
                .WhereValid()) {
                if (obj == null) {
                    obj = new();
                }
                if (!symbol.BeginValidation()
                    .IsClass<INamedTypeSymbol>()
                    .IsNotGenericType<INamedTypeSymbol>()
                    .IsNotAbstractType<INamedTypeSymbol>()
                    .IsPublic()
                    .IsDefaultConstructible<INamedTypeSymbol>()
                    .Require(ctx.Diagnostics)) {
                    continue;
                }
                obj.ObjectType = symbol;
                if (!obj.ParseAttributeData(ctx, attr)) {
                    continue;
                }
                objs.Add(obj);
                obj = null;
            }
            Objects = objs.OrderBy(o => o.Priority).Select((o, i) => {
                o.Index = i;
                return o;
            }).ToArray();
        }
    }
}
