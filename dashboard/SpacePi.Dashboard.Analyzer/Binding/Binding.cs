using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// A binding declared on a property
    /// </summary>
    public record Binding {
        /// <summary>
        /// The different types of supported bindings
        /// </summary>
        public enum Modes {
            /// <summary>
            /// A binding to exactly one target
            /// </summary>
            Single,
            /// <summary>
            /// A binding to a list of targets
            /// </summary>
            List,
            /// <summary>
            /// A binding to a list of targets, stored in a dictionary
            /// </summary>
            Dictionary,
            /// <summary>
            /// A binding to the factory instance
            /// </summary>
            Factory
        }

        /// <summary>
        /// The type of key stored in a dictionary binding
        /// </summary>
        public enum DictionaryKeyType {
            Int,
            String
        }

        /// <summary>
        /// The property on which the binding is applied
        /// </summary>
        public IPropertySymbol Symbol;
        /// <summary>
        /// The ID of the target object
        /// </summary>
        public string Id;
        /// <summary>
        /// The binding mode
        /// </summary>
        public Modes Mode;
        /// <summary>
        /// The name of the property which is the key of the dictionary
        /// </summary>
        public string DictionaryKey;
        /// <summary>
        /// The factory the binding is targetting
        /// </summary>
        public BindingFactory Factory;
        /// <summary>
        /// A list of objects the binding is targetting
        /// </summary>
        public FactoryObject[] Objects;
        /// <summary>
        /// The type of key used in the dictionary
        /// </summary>
        public DictionaryKeyType KeyType;
        /// <summary>
        /// A dictionary of objects the binding is targetting
        /// </summary>
        public IDictionaryHasher<FactoryObject> ObjectDict;
        /// <summary>
        /// A set of parameter filters on the binding
        /// </summary>
        public List<(string paramName, string match)> ParameterFilters;

        /// <summary>
        /// Parses all declared bindings in a class
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="factory">The binding factory</param>
        /// <param name="obj">The object to parse bindings for</param>
        /// <returns>A list of bindings</returns>
        public static IEnumerable<Binding> ParseAll(Context ctx, BindingFactory factory, FactoryObject obj) {
            Binding binding = null;
            foreach ((IPropertySymbol prop, AttributeData attr) in obj.ObjectType.GetAllMembers()
                .OfType<IPropertySymbol>()
                .BeginValidation()
                .HasAttribute(factory.BindingAttribute, a => a)
                .WhereValid()) {
                if (binding == null) {
                    binding = new();
                }
                if (!prop.BeginValidation()
                    .HasPublicSetter<IPropertySymbol>()
                    .Require(ctx.Diagnostics)) {
                    continue;
                }
                binding.Symbol = prop;
                binding.Id = null;
                binding.ParameterFilters = new();
                foreach ((TypedConstant val, IParameterSymbol param) in attr.ConstructorArguments
                    .Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))) {
                    if (param.BeginValidation().HasAttribute(ctx.BindingIDAttribute).Check()) {
                        binding.Id = val.Value.ToString();
                    }
                    if (param.BeginValidation().HasAttribute(ctx.BindingMatchIDAttribute).Check()) {
                        binding.ParameterFilters.Add((val.Value.ToString(), obj.Id));
                    }
                }
                binding.Id = attr.ConstructorArguments.Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))
                    .Where(t => t.b.BeginValidation().HasAttribute(ctx.BindingIDAttribute).Check())
                    .Select(t => t.a.Value.ToString())
                    .FirstOrDefault();
                if (prop.Type.DEquals(factory.Symbol)) {
                    binding.Mode = Modes.Factory;
                } else if (prop.Type.OriginalDefinition.DEquals(ctx.IDictionary_2)) {
                    binding.Mode = Modes.Dictionary;
                    binding.DictionaryKey = attr.ConstructorArguments.Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))
                        .Where(t => t.b.BeginValidation().HasAttribute(ctx.BindingDictionaryKeyAttribute).Check())
                        .Select(t => t.a.Value.ToString())
                        .FirstOrDefault();
                    if (binding.DictionaryKey == null) {
                        ctx.Diagnostics.DictionaryBindingMustHaveKey.Report(prop, prop);
                        continue;
                    }
                    bool valid = true;
                    switch (((INamedTypeSymbol) binding.Symbol.Type).TypeArguments[0].SpecialType) {
                        case SpecialType.System_Int32:
                            binding.KeyType = DictionaryKeyType.Int;
                            binding.ObjectDict = new DictionaryHasher<int, FactoryObject>(new PrimitiveEqualityComparers.Int());
                            break;
                        case SpecialType.System_String:
                            binding.KeyType = DictionaryKeyType.String;
                            binding.ObjectDict = new DictionaryHasher<string, FactoryObject>(new PrimitiveEqualityComparers.String());
                            break;
                        default:
                            ctx.Diagnostics.UnsupportedKeyType.Report(prop, prop, ((INamedTypeSymbol) binding.Symbol.Type).TypeArguments[0].SpecialType);
                            valid = false;
                            break;
                    }
                    if (!valid) {
                        continue;
                    }
                } else if (prop.Type.OriginalDefinition.DEquals(ctx.IEnumerable_1)) {
                    binding.Mode = Modes.List;
                } else {
                    binding.Mode = Modes.Single;
                }
                binding.Factory = factory;
                yield return binding;
                binding = null;
            }
        }

        /// <summary>
        /// Finds targets for a list of bindings
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="bindings">The bindings to bind</param>
        public static void PerformBinding(Context ctx, IEnumerable<Binding> bindings) {
            foreach (Binding binding in bindings) {
                binding.Objects = null;
                if (binding.Mode == Modes.Factory) {
                    // Nothing to bind to explicitly
                    continue;
                }
                ITypeSymbol type = binding.Symbol.Type;
                if (binding.Mode == Modes.List) {
                    type = ((INamedTypeSymbol) type).TypeArguments[0];
                } else if (binding.Mode == Modes.Dictionary) {
                    type = ((INamedTypeSymbol) type).TypeArguments[1];
                }
                IEnumerable<FactoryObject> filter = binding.Factory.Objects
                    .Where(o => ((ITypeSymbol) o.ObjectType).BeginValidation()
                        .IsCastableTo(type)
                        .Check() &&
                        binding.ParameterFilters.All(f =>
                            o.Parameters.TryGetValue(f.paramName, out TypedConstant v) &&
                            v.Value?.ToString() == f.match));
                if (binding.Id != null) {
                    filter = filter.Where(o => o.Id == binding.Id);
                }
                if (binding.Mode == Modes.Single) {
                    filter = filter.Take(2);
                }
                binding.Objects = filter.ToArray();
                if (binding.Mode == Modes.Single && binding.Objects.Length != 1) {
                    ctx.Diagnostics.ExactlyOneBindingTargetNotFound.Report(binding.Symbol, binding.Objects.Length, binding.Symbol);
                }
                if (binding.Mode == Modes.Dictionary) {
                    foreach (FactoryObject obj in binding.Objects) {
                        if (obj.Parameters.TryGetValue(binding.DictionaryKey, out TypedConstant val)) {
                            binding.ObjectDict.Add(val, obj);
                        } else {
                            ctx.Diagnostics.BindingMissingKey.Report(binding.Symbol, binding.Symbol, obj.ObjectType);
                        }
                    }
                }
            }
        }
    }
}
