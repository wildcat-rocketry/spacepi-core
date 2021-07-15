using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record BindingBoxer {
        public enum ParameterType {
            FactoryObject,
            ID,
            Parameter,
            Priority
        }

        public IMethodSymbol Method;
        public Parameter[] Parameters;
        public IMethodSymbol Unboxer;

        public void TryParse(INamedTypeSymbol factory, BindingArray array, Context ctx) {
            Method = factory.GetAllMembers()
                .OfType<IMethodSymbol>()
                .BeginValidation()
                .HasAttribute(ctx.BindingFactoryBoxerAttribute)
                .IsPublic()
                .RequireOne(ctx.Diagnostics, "factory object boxer method", factory)
                .StripContext();
            if (Method != null) {
                Parameters = Method.Parameters
                    .Select(p => (p, new[] {
                            (ctx.BindingFactoryObjectAttribute, Parameter.Types.FactoryObject),
                            (ctx.BindingIDAttribute, Parameter.Types.ID),
                            (ctx.BindingParameterAttribute, Parameter.Types.Parameter),
                            (ctx.BindingPriorityAttribute, Parameter.Types.Priority)
                        }.Select(t => (p.GetAttributes().FirstOrDefault(a => a.AttributeClass.DEquals(t.Item1)), t.Item2))
                            .Where(t => t.Item1 != null)
                            .Take(2)
                            .ToArray()
                    )).Select(t => {
                        (AttributeData Attribute, Parameter.Types Type)[] attrs = t.Item2;
                        if (attrs.Length != 1) {
                            ctx.Diagnostics.FactoryConstructorParameterAttributes.Report(t.p);
                            return null;
                        } else {
                            Parameter p = new() {
                                Symbol = t.p,
                                Type = attrs[0].Type
                            };
                            if (p.Type == Parameter.Types.Parameter) {
                                p.Name = attrs[0].Attribute.ConstructorArguments[0].Value.ToString();
                            }
                            return p;
                        }
                    }).ToArray();
                Unboxer = factory.GetAllMembers()
                    .OfType<IMethodSymbol>()
                    .BeginValidation()
                    .HasAttribute(ctx.BindingFactoryUnboxerAttribute)
                    .IsPublic()
                    .RequireOne(ctx.Diagnostics, "factory object unboxer method", factory)
                    .StripContext();
            }
        }
    }
}
