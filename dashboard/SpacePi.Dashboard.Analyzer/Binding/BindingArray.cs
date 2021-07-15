using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record BindingArray {
        public INamedTypeSymbol Type;
        public IPropertySymbol Property;

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
