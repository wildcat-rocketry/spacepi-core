using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// Helper to validate properties of a symbol
    /// </summary>
    /// <typeparam name="TSymbol">The type of symbol</typeparam>
    /// <typeparam name="TContext">The type of context to pass along with it</typeparam>
    public class Validator<TSymbol, TContext> where TSymbol : ISymbol {
        /// <summary>
        /// The symbol
        /// </summary>
        public readonly TSymbol Symbol;
        /// <summary>
        /// The diagnostics to report as a result of the validator
        /// </summary>
        public readonly List<Action<Diagnostics>> Diagnostics;
        /// <summary>
        /// The context object
        /// </summary>
        public TContext Context;

        public bool Check() => Diagnostics.Count == 0;

        public (bool, TContext) CheckWithContext() => (Check(), Context);

        public bool Require(Diagnostics diags) {
            foreach (Action<Diagnostics> diag in Diagnostics) {
                diag(diags);
            }
            return Check();
        }

        public (bool, TContext) RequireWithContext(Diagnostics diags) => (Require(diags), Context);

        private Validator<TSymbol, TContext> _(bool cond, Action<Diagnostics> diag) {
            if (!cond) {
                Diagnostics.Add(diag);
            }
            return this;
        }

        public Validator(TSymbol symbol, TContext ctx = default) : this(symbol, new(), ctx) {
        }

        private Validator(TSymbol symbol, List<Action<Diagnostics>> diags, TContext ctx = default) {
            Symbol = symbol;
            Diagnostics = diags;
            Context = ctx;
        }

        // ISymbol Validators
        public Validator<TSymbol, TContext> IsPublic() =>
            _(Symbol.DeclaredAccessibility == Accessibility.Public, d => d.IsPublic.Report(Symbol, Symbol));

        public Validator<TSymbol, TContextOut> HasAttribute<TContextOut>(INamedTypeSymbol attr, Func<AttributeData, TContext, TContextOut> select) {
            AttributeData[] attrs = Symbol.GetAttributes().Where(a => a.AttributeClass.DEquals(attr)).Take(2).ToArray();
            if (attrs.Length != 1) {
                Diagnostics.Add(d => d.OneRequired.Report(attr.Name, Symbol));
                return new(Symbol, Diagnostics);
            } else {
                return new(Symbol, Diagnostics, select(attrs[0], Context));
            }
        }

        public Validator<TSymbol, TContextOut> HasAttribute<TContextOut>(INamedTypeSymbol attr, Func<AttributeData, TContextOut> select) =>
            HasAttribute(attr, (a, _) => select(a));

        public Validator<TSymbol, TContext> HasAttribute(INamedTypeSymbol attr) =>
            HasAttribute(attr, (_, t) => t);

        public Validator<TSymbol, TContext> ContextTrue<TTContext>(string message = null) where TTContext : TContext, IEquatable<bool> =>
            _(Context.Equals(true), d => d.ContextTrue.Report(Symbol, message));

        public Validator<TSymbol, TContext> IsInstance() =>
            _(!Symbol.IsStatic, d => d.IsInstance.Report(Symbol, Symbol));

        // ITypeSymbol Validators
        public Validator<TSymbol, TContext> IsClass<TTSymbol>() where TTSymbol : TSymbol, ITypeSymbol =>
            _(((ITypeSymbol) Symbol).TypeKind == TypeKind.Class, d => d.IsClass.Report(Symbol, Symbol));

        public Validator<TSymbol, TContext> IsNotAbstractType<TTSymbol>() where TTSymbol : TSymbol, ITypeSymbol =>
            _(!((ITypeSymbol) Symbol).IsAbstract, d => d.IsNotAbstract.Report(Symbol, Symbol));

        public Validator<TSymbol, TContext> Extends<TTSymbol>(TTSymbol baseType) where TTSymbol : TSymbol, ITypeSymbol =>
            _(((ITypeSymbol) Symbol).BaseType.DEquals(baseType), d => d.Extends.Report(Symbol, Symbol, baseType));

        public Validator<TSymbol, TContext> Implements<TTSymbol>(TTSymbol iface) where TTSymbol : TSymbol, ITypeSymbol =>
            _(((ITypeSymbol) Symbol).AllInterfaces.Any(i => i.DEquals(iface)), d => d.Implements.Report(Symbol, Symbol, iface));

        public Validator<TSymbol, TContext> IsCastableTo<TTSymbol>(TTSymbol baseType) where TTSymbol : TSymbol, ITypeSymbol =>
            Symbol.DEquals(baseType) ? this : baseType.TypeKind == TypeKind.Interface ? Implements(baseType) : Extends(baseType);

        public Validator<TSymbol, TContext> IsArray<TTSymbol>() where TTSymbol : TSymbol, ITypeSymbol =>
            _(Symbol is IArrayTypeSymbol, d => d.IsArray.Report(Symbol, Symbol));

        // INamedTypeSymbol Validators
        public Validator<TSymbol, TContext> IsNotGenericType<TTSymbol>() where TTSymbol : TSymbol, INamedTypeSymbol =>
            _(!((INamedTypeSymbol) Symbol).IsGenericType, d => d.IsNotGeneric.Report(Symbol, Symbol));

        public Validator<TSymbol, TContext> IsDefaultConstructible<TTSymbol>() where TTSymbol : TSymbol, INamedTypeSymbol =>
            _(((INamedTypeSymbol) Symbol).InstanceConstructors
                .BeginValidation()
                .IsPublic()
                .IsParameterless()
                .WhereValid()
                .Any(), d => d.IsDefaultConstructible.Report(Symbol, Symbol));

        // IMethodSymbol Validators
        public Validator<TSymbol, TContext> IsOrdinaryMethod<TTSymbol>() where TTSymbol : TSymbol, IMethodSymbol =>
            _(((IMethodSymbol) Symbol).MethodKind == MethodKind.Ordinary, d => d.IsOrdinaryMethod.Report(Symbol, Symbol));

        public Validator<TSymbol, TContext> IsParameterless<TTSymbol>() where TTSymbol : TSymbol, IMethodSymbol =>
            _(((IMethodSymbol) Symbol).Parameters.IsEmpty, d => d.IsParameterless.Report(Symbol, Symbol));

        // IPropertySymbol Validators
        public Validator<TSymbol, TContext> HasPublicGetter<TTSymbol>() where TTSymbol : TSymbol, IPropertySymbol =>
            _(((IPropertySymbol) Symbol).GetMethod?.BeginValidation()?.IsPublic()?.Check() == true, d => d.HasPublicGetterSetter.Report(Symbol, Symbol, "getter"));

        public Validator<TSymbol, TContext> HasPublicSetter<TTSymbol>() where TTSymbol : TSymbol, IPropertySymbol =>
            _(((IPropertySymbol) Symbol).SetMethod?.BeginValidation()?.IsPublic()?.Check() == true, d => d.HasPublicGetterSetter.Report(Symbol, Symbol, "setter"));
    }

    /// <summary>
    /// Extension methods to assist syntax with the validator
    /// </summary>
    public static class ValidatorExtensions {
        // Begin Methods
        public static Validator<TSymbol, TContext> BeginValidation<TSymbol, TContext>(this TSymbol t, TContext ctx) where TSymbol : ISymbol =>
            new(t, ctx);

        public static Validator<TSymbol, object> BeginValidation<TSymbol>(this TSymbol t) where TSymbol : ISymbol =>
            new(t, null);

        public static IEnumerable<Validator<TSymbol, TContext>> BeginValidation<TSymbol, TContext>(this IEnumerable<(TSymbol, TContext)> t) where TSymbol : ISymbol =>
            t.Select(s => s.Item1.BeginValidation(s.Item2));

        public static IEnumerable<Validator<TSymbol, object>> BeginValidation<TSymbol>(this IEnumerable<TSymbol> t) where TSymbol : ISymbol =>
            t.Select(s => s.BeginValidation());

        // End Methods
        public static IEnumerable<(bool Valid, TSymbol Symbol, TContext Context)> Check<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ISymbol =>
            t.Select(v => (v.Check(), v.Symbol, v.Context));

        public static IEnumerable<(TSymbol Symbol, TContext Context)> WhereValid<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ISymbol =>
            t.Check().Where(t => t.Valid).Select(t => (t.Symbol, t.Context));

        public static IEnumerable<(TSymbol Symbol, TContext Context)> Require<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, Diagnostics diags) where TSymbol : ISymbol =>
            t.Select(v => (v.Require(diags), v.Symbol, v.Context)).Where(t => t.Item1).Select(t => (t.Symbol, t.Context));

        public static (TSymbol Symbol, TContext Context) CheckOne<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ISymbol {
            (TSymbol, TContext)[] candidates = t.WhereValid().Take(2).ToArray();
            if (candidates.Length == 1) {
                return candidates[0];
            }
            return default;
        }

        public static (TSymbol Symbol, TContext Context) RequireOne<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, Diagnostics diags, string desc, ISymbol parent) where TSymbol : ISymbol {
            (TSymbol, TContext) symbol = t.CheckOne();
            if (symbol.Item1 == null) {
                diags.OneRequired.Report(parent, desc, parent);
            }
            return symbol;
        }

        public static TSymbol StripContext<TSymbol, TContext>(this (TSymbol, TContext) t) where TSymbol : ISymbol =>
            t.Item1;

        public static IEnumerable<TSymbol> StripContext<TSymbol, TContext>(this IEnumerable<(TSymbol, TContext)> t) where TSymbol : ISymbol =>
            t.Select(s => s.Item1);

        // ISymbol Validators
        public static IEnumerable<Validator<TSymbol, TContext>> IsPublic<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ISymbol =>
            t.Select(t => t.IsPublic());

        public static IEnumerable<Validator<TSymbol, TContextOut>> HasAttribute<TSymbol, TContext, TContextOut>(this IEnumerable<Validator<TSymbol, TContext>> t, INamedTypeSymbol attr, Func<AttributeData, TContext, TContextOut> select) where TSymbol : ISymbol =>
            t.Select(t => t.HasAttribute(attr, select));

        public static IEnumerable<Validator<TSymbol, TContextOut>> HasAttribute<TSymbol, TContext, TContextOut>(this IEnumerable<Validator<TSymbol, TContext>> t, INamedTypeSymbol attr, Func<AttributeData, TContextOut> select) where TSymbol : ISymbol =>
            t.Select(t => t.HasAttribute(attr, select));

        public static IEnumerable<Validator<TSymbol, TContext>> HasAttribute<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, INamedTypeSymbol attr) where TSymbol : ISymbol =>
            t.Select(t => t.HasAttribute(attr));

        public static IEnumerable<Validator<TSymbol, TContext>> ContextTrue<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, string message = null) where TSymbol : ISymbol where TContext : IEquatable<bool> =>
            t.Select(t => t.ContextTrue<TContext>(message));

        public static IEnumerable<Validator<TSymbol, TContext>> IsInstance<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ISymbol =>
            t.Select(t => t.IsInstance());

        // ITypeSymbol Validators
        public static IEnumerable<Validator<TSymbol, TContext>> IsClass<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ITypeSymbol =>
            t.Select(t => t.IsClass<TSymbol>());

        public static IEnumerable<Validator<TSymbol, TContext>> IsNotAbstractType<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ITypeSymbol =>
            t.Select(t => t.IsNotAbstractType<TSymbol>());

        public static IEnumerable<Validator<TSymbol, TContext>> Extends<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, TSymbol baseType) where TSymbol : ITypeSymbol =>
            t.Select(t => t.Extends(baseType));

        public static IEnumerable<Validator<TSymbol, TContext>> Implements<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, TSymbol iface) where TSymbol : ITypeSymbol =>
            t.Select(t => t.Implements(iface));

        public static IEnumerable<Validator<TSymbol, TContext>> IsCastableTo<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t, TSymbol baseType) where TSymbol : ITypeSymbol =>
            t.Select(t => t.IsCastableTo(baseType));

        public static IEnumerable<Validator<TSymbol, TContext>> IsArray<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : ITypeSymbol =>
            t.Select(t => t.IsArray<TSymbol>());

        // INamedTypeSymbol Validators
        public static IEnumerable<Validator<TSymbol, TContext>> IsNotGenericType<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : INamedTypeSymbol =>
            t.Select(t => t.IsNotGenericType<TSymbol>());

        public static IEnumerable<Validator<TSymbol, TContext>> IsDefaultConstructible<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : INamedTypeSymbol =>
            t.Select(t => t.IsDefaultConstructible<TSymbol>());

        // IMethodSymbol Validators
        public static IEnumerable<Validator<TSymbol, TContext>> IsOrdinaryMethod<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : IMethodSymbol =>
            t.Select(t => t.IsOrdinaryMethod<TSymbol>());

        public static IEnumerable<Validator<TSymbol, TContext>> IsParameterless<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : IMethodSymbol =>
            t.Select(t => t.IsParameterless<TSymbol>());

        // IPropertySymbol Validators
        public static IEnumerable<Validator<TSymbol, TContext>> HasPublicGetter<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : IPropertySymbol =>
            t.Select(t => t.HasPublicGetter<TSymbol>());

        public static IEnumerable<Validator<TSymbol, TContext>> HasPublicSetter<TSymbol, TContext>(this IEnumerable<Validator<TSymbol, TContext>> t) where TSymbol : IPropertySymbol =>
            t.Select(t => t.HasPublicSetter<TSymbol>());
    }
}
