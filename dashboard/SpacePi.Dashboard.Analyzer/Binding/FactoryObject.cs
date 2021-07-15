using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record FactoryObject {
        public INamedTypeSymbol ObjectType;
        public int Index;
        public string Id;
        public Dictionary<string, TypedConstant> Parameters = new();
        public int Priority;
        public Binding[] Bindings;

        public bool ParseAttributeData(Context ctx, AttributeData attr) {
            Parameters.Clear();
            if (attr.ConstructorArguments.Length != attr.AttributeConstructor.Parameters.Length) {
                ctx.Diagnostics.DefaultArgsNotSupported.Report(attr.AttributeConstructor);
                return false;
            }
            foreach ((TypedConstant val, IParameterSymbol param) in attr.ConstructorArguments.Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))) {
                bool isID = param.BeginValidation().HasAttribute(ctx.BindingIDAttribute).Check();
                (bool isParameter, AttributeData paramAttr) = param.BeginValidation().HasAttribute(ctx.BindingParameterAttribute, a => a).CheckWithContext();
                bool isPriority = param.BeginValidation().HasAttribute(ctx.BindingPriorityAttribute).Check();
                if ((isID && isParameter) | (isID && isPriority) || (isParameter && isPriority)) {
                    ctx.Diagnostics.DeclarativeAttributeCtorMultipleParam.Report(param);
                    return false;
                }
                if (isID) {
                    if (val.Type.SpecialType != SpecialType.System_String) {
                        ctx.Diagnostics.DeclarativeAttributeParameterBadType.Report(param, "string");
                        return false;
                    }
                    Id = val.Value.ToString();
                } else if (isParameter) {
                    string name = paramAttr.ConstructorArguments[0].Value.ToString();
                    if (Parameters.ContainsKey(name)) {
                        ctx.Diagnostics.DeclarativeAttributeCtorMultipleParam.Report(param);
                        return false;
                    }
                    Parameters[name] = val;
                } else if (isPriority) {
                    if (val.Type.SpecialType != SpecialType.System_Int32) {
                        ctx.Diagnostics.DeclarativeAttributeParameterBadType.Report(param, "int");
                        return false;
                    }
                    Priority = (int) val.Value;
                }
            }
            return true;
        }

        public void ParseBindings(Context ctx, IEnumerable<BindingFactory> factories) =>
            Bindings = factories.SelectMany(f => Binding.ParseAll(ctx, f, ObjectType)).ToArray();
    }
}
