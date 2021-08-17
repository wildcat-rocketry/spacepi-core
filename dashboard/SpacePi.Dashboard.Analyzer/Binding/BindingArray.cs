using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// The array property stored in a factory
    /// </summary>
    public record BindingArray {
        /// <summary>
        /// The type of element stored in the array
        /// </summary>
        public INamedTypeSymbol Type;
        /// <summary>
        /// The property symbol declaring the array
        /// </summary>
        public IPropertySymbol Property;

        /// <summary>
        /// Tries to find an array declared inside a factory class
        /// </summary>
        /// <param name="factory">The factory class</param>
        /// <param name="ctx">The compilation context</param>
        public void TryParse(INamedTypeSymbol factory, Context ctx) {
            (Property, Type) = factory.GetAllMembers()
                .OfType<IPropertySymbol>()
                .Where(p => p.Type.BeginValidation().IsArray<ITypeSymbol>().Check())
                .BeginValidation()
                .HasAttribute(ctx.BindingFactoryArrayAttribute, a => (INamedTypeSymbol) a.ConstructorArguments[0].Value)
                .HasPublicGetter()
                .HasPublicSetter()
                .RequireOne(ctx.Diagnostics, "factory array", factory);
        }
    }
}
