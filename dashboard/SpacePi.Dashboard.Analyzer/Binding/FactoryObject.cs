using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// An object constructed by a factory
    /// </summary>
    public record FactoryObject {
        /// <summary>
        /// The type of the object
        /// </summary>
        public INamedTypeSymbol ObjectType;
        /// <summary>
        /// The index of the object in its array
        /// </summary>
        public int Index;
        /// <summary>
        /// The ID of the object
        /// </summary>
        public string Id;
        /// <summary>
        /// The values of each parameter
        /// </summary>
        public Dictionary<string, TypedConstant> Parameters = new();
        /// <summary>
        /// The priority of this instance
        /// </summary>
        public int Priority;
        /// <summary>
        /// A list of bindings within the instance
        /// </summary>
        public Binding[] Bindings;

        /// <summary>
        /// Parses data from the attribute annotating an object instance
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="attr">The attribute data</param>
        /// <returns>If the parsing was completed successfully</returns>
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
                if (isID) {
                    if (val.Type.SpecialType != SpecialType.System_String) {
                        ctx.Diagnostics.DeclarativeAttributeParameterBadType.Report(param, "string");
                        return false;
                    }
                    Id = val.Value.ToString();
                }
                if (isParameter) {
                    string name = paramAttr.ConstructorArguments[0].Value.ToString();
                    if (Parameters.ContainsKey(name)) {
                        ctx.Diagnostics.DeclarativeAttributeCtorMultipleParam.Report(param, name);
                        return false;
                    }
                    Parameters[name] = val;
                }
                if (isPriority) {
                    if (val.Type.SpecialType != SpecialType.System_Int32) {
                        ctx.Diagnostics.DeclarativeAttributeParameterBadType.Report(param, "int");
                        return false;
                    }
                    Priority = (int) val.Value;
                }
            }
            return true;
        }

        /// <summary>
        /// Parses all bindings declared in this object
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="factories">All factories defined in the source</param>
        public void ParseBindings(Context ctx, IEnumerable<BindingFactory> factories) =>
            Bindings = factories.SelectMany(f => Binding.ParseAll(ctx, f, ObjectType)).ToArray();
    }
}
