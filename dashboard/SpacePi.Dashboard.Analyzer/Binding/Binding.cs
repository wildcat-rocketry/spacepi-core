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
        /// Parses all declared bindings in a class
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        /// <param name="factory"></param>
        /// <param name="cls"></param>
        /// <returns></returns>
        public static IEnumerable<Binding> ParseAll(Context ctx, BindingFactory factory, INamedTypeSymbol cls) {
            Binding obj = null;
            foreach ((IPropertySymbol prop, AttributeData attr) in cls.GetAllMembers()
                .OfType<IPropertySymbol>()
                .BeginValidation()
                .HasAttribute(factory.BindingAttribute, a => a)
                .WhereValid()) {
                if (obj == null) {
                    obj = new();
                }
                if (!prop.BeginValidation()
                    .HasPublicSetter<IPropertySymbol>()
                    .Require(ctx.Diagnostics)) {
                    continue;
                }
                obj.Symbol = prop;
                obj.Id = attr.ConstructorArguments.Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))
                    .Where(t => t.b.BeginValidation().HasAttribute(ctx.BindingIDAttribute).Check())
                    .Select(t => t.a.Value.ToString())
                    .FirstOrDefault();
                if (prop.Type.DEquals(factory.Symbol)) {
                    obj.Mode = Modes.Factory;
                } else if (prop.Type.OriginalDefinition.DEquals(ctx.IDictionary_2)) {
                    obj.Mode = Modes.Dictionary;
                    obj.DictionaryKey = attr.ConstructorArguments.Zip(attr.AttributeConstructor.Parameters, (a, b) => (a, b))
                        .Where(t => t.b.BeginValidation().HasAttribute(ctx.BindingDictionaryKeyAttribute).Check())
                        .Select(t => t.a.Value.ToString())
                        .FirstOrDefault();
                    if (obj.DictionaryKey == null) {
                        ctx.Diagnostics.DictionaryBindingMustHaveKey.Report(prop, prop);
                        continue;
                    }
                    bool valid = true;
                    switch (((INamedTypeSymbol) obj.Symbol.Type).TypeArguments[0].SpecialType) {
                        case SpecialType.System_Int32:
                            obj.KeyType = DictionaryKeyType.Int;
                            obj.ObjectDict = new DictionaryHasher<int, FactoryObject>(new PrimitiveEqualityComparers.Int());
                            break;
                        case SpecialType.System_String:
                            obj.KeyType = DictionaryKeyType.String;
                            obj.ObjectDict = new DictionaryHasher<string, FactoryObject>(new PrimitiveEqualityComparers.String());
                            break;
                        default:
                            ctx.Diagnostics.UnsupportedKeyType.Report(prop, prop, ((INamedTypeSymbol) obj.Symbol.Type).TypeArguments[0].SpecialType);
                            valid = false;
                            break;
                    }
                    if (!valid) {
                        continue;
                    }
                } else if (prop.Type.OriginalDefinition.DEquals(ctx.IEnumerable_1)) {
                    obj.Mode = Modes.List;
                } else {
                    obj.Mode = Modes.Single;
                }
                obj.Factory = factory;
                yield return obj;
                obj = null;
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
                        .Check());
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
