using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    [SuppressMessage("MicrosoftCodeAnalysisCompatibility", "RS1009:Only internal implementations of this interface are allowed", Justification = "Need implementation to mock future IPluginFactory partial class code")]
    public class PluginFactorySymbol : INamedTypeSymbol {
        private readonly INamedTypeSymbol Base;

        public int Arity => Base.Arity;

        public bool IsGenericType => Base.IsGenericType;

        public bool IsUnboundGenericType => Base.IsUnboundGenericType;

        public bool IsScriptClass => Base.IsScriptClass;

        public bool IsImplicitClass => Base.IsImplicitClass;

        public bool IsComImport => Base.IsComImport;

        public IEnumerable<string> MemberNames => Base.MemberNames;

        public ImmutableArray<ITypeParameterSymbol> TypeParameters => Base.TypeParameters;

        public ImmutableArray<ITypeSymbol> TypeArguments => Base.TypeArguments;

        public ImmutableArray<NullableAnnotation> TypeArgumentNullableAnnotations => Base.TypeArgumentNullableAnnotations;

        public INamedTypeSymbol OriginalDefinition => Base.OriginalDefinition;

        public IMethodSymbol DelegateInvokeMethod => Base.DelegateInvokeMethod;

        public INamedTypeSymbol EnumUnderlyingType => Base.EnumUnderlyingType;

        public INamedTypeSymbol ConstructedFrom => Base.ConstructedFrom;

        public ImmutableArray<IMethodSymbol> InstanceConstructors => Base.InstanceConstructors;

        public ImmutableArray<IMethodSymbol> StaticConstructors => Base.StaticConstructors;

        public ImmutableArray<IMethodSymbol> Constructors => Base.Constructors;

        public ISymbol AssociatedSymbol => Base.AssociatedSymbol;

        public bool MightContainExtensionMethods => Base.MightContainExtensionMethods;

        public INamedTypeSymbol TupleUnderlyingType => Base.TupleUnderlyingType;

        public ImmutableArray<IFieldSymbol> TupleElements => Base.TupleElements;

        public bool IsSerializable => Base.IsSerializable;

        public INamedTypeSymbol NativeIntegerUnderlyingType => Base.NativeIntegerUnderlyingType;

        public TypeKind TypeKind => Base.TypeKind;

        public INamedTypeSymbol BaseType => Base.BaseType;

        public ImmutableArray<INamedTypeSymbol> Interfaces => Base.Interfaces;

        public ImmutableArray<INamedTypeSymbol> AllInterfaces { get; }

        public bool IsReferenceType => Base.IsReferenceType;

        public bool IsValueType => Base.IsValueType;

        public bool IsAnonymousType => Base.IsAnonymousType;

        public bool IsTupleType => Base.IsTupleType;

        public bool IsNativeIntegerType => Base.IsNativeIntegerType;

        public SpecialType SpecialType => Base.SpecialType;

        public bool IsRefLikeType => Base.IsRefLikeType;

        public bool IsUnmanagedType => Base.IsUnmanagedType;

        public bool IsReadOnly => Base.IsReadOnly;

        public bool IsRecord => Base.IsRecord;

        public NullableAnnotation NullableAnnotation => Base.NullableAnnotation;

        public bool IsNamespace => Base.IsNamespace;

        public bool IsType => Base.IsType;

        public SymbolKind Kind => Base.Kind;

        public string Language => Base.Language;

        public string Name => Base.Name;

        public string MetadataName => Base.MetadataName;

        public ISymbol ContainingSymbol => Base.ContainingSymbol;

        public IAssemblySymbol ContainingAssembly => Base.ContainingAssembly;

        public IModuleSymbol ContainingModule => Base.ContainingModule;

        public INamedTypeSymbol ContainingType => Base.ContainingType;

        public INamespaceSymbol ContainingNamespace => Base.ContainingNamespace;

        public bool IsDefinition => Base.IsDefinition;

        public bool IsStatic => Base.IsStatic;

        public bool IsVirtual => Base.IsVirtual;

        public bool IsOverride => Base.IsOverride;

        public bool IsAbstract => Base.IsAbstract;

        public bool IsSealed => Base.IsSealed;

        public bool IsExtern => Base.IsExtern;

        public bool IsImplicitlyDeclared => Base.IsImplicitlyDeclared;

        public bool CanBeReferencedByName => Base.CanBeReferencedByName;

        public ImmutableArray<Location> Locations => Base.Locations;

        public ImmutableArray<SyntaxReference> DeclaringSyntaxReferences => Base.DeclaringSyntaxReferences;

        public Accessibility DeclaredAccessibility => Base.DeclaredAccessibility;

        public bool HasUnsupportedMetadata => Base.HasUnsupportedMetadata;

        ITypeSymbol ITypeSymbol.OriginalDefinition => ((ITypeSymbol)Base).OriginalDefinition;

        ISymbol ISymbol.OriginalDefinition => ((ISymbol)Base).OriginalDefinition;

        public void Accept(SymbolVisitor visitor) {
            Base.Accept(visitor);
        }

        public TResult Accept<TResult>(SymbolVisitor<TResult> visitor) {
            return Base.Accept(visitor);
        }

        public INamedTypeSymbol Construct(params ITypeSymbol[] typeArguments) {
            return Base.Construct(typeArguments);
        }

        public INamedTypeSymbol Construct(ImmutableArray<ITypeSymbol> typeArguments, ImmutableArray<NullableAnnotation> typeArgumentNullableAnnotations) {
            return Base.Construct(typeArguments, typeArgumentNullableAnnotations);
        }

        public INamedTypeSymbol ConstructUnboundGenericType() {
            return Base.ConstructUnboundGenericType();
        }

        public bool Equals(ISymbol other, SymbolEqualityComparer equalityComparer) {
            return Base.Equals(other, equalityComparer);
        }

        [SuppressMessage("MicrosoftCodeAnalysisCorrectness", "RS1024:Compare symbols correctly", Justification = "Suppressed in ISymbol as well")]
        public bool Equals(ISymbol other) {
            return ((IEquatable<ISymbol>)Base).Equals(other);
        }

        public ISymbol FindImplementationForInterfaceMember(ISymbol interfaceMember) {
            return Base.FindImplementationForInterfaceMember(interfaceMember);
        }

        public ImmutableArray<AttributeData> GetAttributes() {
            return Base.GetAttributes();
        }

        public string GetDocumentationCommentId() {
            return Base.GetDocumentationCommentId();
        }

        public string GetDocumentationCommentXml(CultureInfo preferredCulture = null, bool expandIncludes = false, CancellationToken cancellationToken = default) {
            return Base.GetDocumentationCommentXml(preferredCulture, expandIncludes, cancellationToken);
        }

        public ImmutableArray<ISymbol> GetMembers() {
            return Base.GetMembers();
        }

        public ImmutableArray<ISymbol> GetMembers(string name) {
            return Base.GetMembers(name);
        }

        public ImmutableArray<CustomModifier> GetTypeArgumentCustomModifiers(int ordinal) {
            return Base.GetTypeArgumentCustomModifiers(ordinal);
        }

        public ImmutableArray<INamedTypeSymbol> GetTypeMembers() {
            return Base.GetTypeMembers();
        }

        public ImmutableArray<INamedTypeSymbol> GetTypeMembers(string name) {
            return Base.GetTypeMembers(name);
        }

        public ImmutableArray<INamedTypeSymbol> GetTypeMembers(string name, int arity) {
            return Base.GetTypeMembers(name, arity);
        }

        public ImmutableArray<SymbolDisplayPart> ToDisplayParts(NullableFlowState topLevelNullability, SymbolDisplayFormat format = null) {
            return Base.ToDisplayParts(topLevelNullability, format);
        }

        public ImmutableArray<SymbolDisplayPart> ToDisplayParts(SymbolDisplayFormat format = null) {
            return Base.ToDisplayParts(format);
        }

        public string ToDisplayString(NullableFlowState topLevelNullability, SymbolDisplayFormat format = null) {
            return Base.ToDisplayString(topLevelNullability, format);
        }

        public string ToDisplayString(SymbolDisplayFormat format = null) {
            return Base.ToDisplayString(format);
        }

        public ImmutableArray<SymbolDisplayPart> ToMinimalDisplayParts(SemanticModel semanticModel, NullableFlowState topLevelNullability, int position, SymbolDisplayFormat format = null) {
            return Base.ToMinimalDisplayParts(semanticModel, topLevelNullability, position, format);
        }

        public ImmutableArray<SymbolDisplayPart> ToMinimalDisplayParts(SemanticModel semanticModel, int position, SymbolDisplayFormat format = null) {
            return Base.ToMinimalDisplayParts(semanticModel, position, format);
        }

        public string ToMinimalDisplayString(SemanticModel semanticModel, NullableFlowState topLevelNullability, int position, SymbolDisplayFormat format = null) {
            return Base.ToMinimalDisplayString(semanticModel, topLevelNullability, position, format);
        }

        public string ToMinimalDisplayString(SemanticModel semanticModel, int position, SymbolDisplayFormat format = null) {
            return Base.ToMinimalDisplayString(semanticModel, position, format);
        }

        public ITypeSymbol WithNullableAnnotation(NullableAnnotation nullableAnnotation) {
            return Base.WithNullableAnnotation(nullableAnnotation);
        }

        public override bool Equals(object obj) {
            return Base.Equals(obj);
        }

        [SuppressMessage("MicrosoftCodeAnalysisCorrectness", "RS1024:Compare symbols correctly", Justification = "Suppressed in ISymbol as well")]
        public override int GetHashCode() {
            return Base.GetHashCode();
        }

        public override string ToString() {
            return Base.ToString();
        }

        public PluginFactorySymbol(INamedTypeSymbol @base, Compilation compilation) {
            Base = @base;
            AllInterfaces = Base.AllInterfaces.Concat(new[] {
                compilation.GetTypeByMetadataName(Types.IDisposable),
                compilation.GetTypeByMetadataName(Types.IPlugin),
                compilation.GetTypeByMetadataName(Types.IPluginFactory)
            }.Where(o => o != null)).ToImmutableArray();
        }
    }
}
