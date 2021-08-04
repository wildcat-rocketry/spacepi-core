using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// The boxing method declared in a factory
    /// </summary>
    public record BindingBoxer {
        /// <summary>
        /// The method symbol used to box an instance
        /// </summary>
        public IMethodSymbol Method;
        /// <summary>
        /// The parameters to the function
        /// </summary>
        public Parameter[] Parameters;
        /// <summary>
        /// The method symbol used to unbox an instance
        /// </summary>
        public IMethodSymbol Unboxer;

        /// <summary>
        /// Tries to find the boxing and unboxing methods in a factory class
        /// </summary>
        /// <param name="factory">The factory class</param>
        /// <param name="ctx">The compilation context</param>
        public void TryParse(INamedTypeSymbol factory, Context ctx) {
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
