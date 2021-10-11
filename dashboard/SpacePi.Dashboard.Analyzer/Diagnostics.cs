using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer {
    public partial class Diagnostics {
        ////////////////////////////////////////////////////////////////////////
        // General Diagnostics
        public readonly D AnalyzerException = new(Error, 0,
            "Analyzer encountered an exception",
            "Analyzer encountered an unhandled {0}: {1}");
        public readonly D APITypeMissing = new(Error, 1,
            "Missing Analyzer API type",
            "Analyzer API type {0} was not found in the current compilation context");

        ////////////////////////////////////////////////////////////////////////
        // Protobuf Diagnostics
        public readonly D CMakeBuildStatus = new(Info, 100,
            "CMake build message",
            "{0}");
        public readonly D CMakeBuildError = new(Error, 101,
            "CMake build error",
            "{0}");
        public readonly D ProtocBuildStatus = new(Info, 102,
            "CMake build message",
            "{0}");
        public readonly D ProtocBuildError = new(Error, 103,
            "CMake build error",
            "{0}");

        ////////////////////////////////////////////////////////////////////////
        // Binding Diagnostics
        public readonly D DefaultArgsNotSupported = new(Error, 200,
            "Default binding attribute arguments not supported",
            "Default binding attribute arguments not supported");
        public readonly D DeclarativeAttributeCtorMultipleParam = new(Error, 201,
            "Attribute constructor has duplicate parameters",
            "Attribute constructor has duplicate parameters with name {0}");
        public readonly D DeclarativeAttributeParameterBadType = new(Error, 202,
            "Bad attribute constructor parameter type",
            "Bad attribute constructor parameter type (expected {0})");
        public readonly D DeclarativeAttributeDuplicateParameter = new(Error, 203,
            "Duplicate attribute constructor parameter name",
            "Duplicate attribute constructor parameter name");
        public readonly D FactoryConstructorParameterAttributes = new(Error, 204,
            "Exactly one of [BindingFactoryObject], [BindingID], [BindingParameter], or [BindingPriority] required",
            "Exactly one of [BindingFactoryObject], [BindingID], [BindingParameter], or [BindingPriority] required");
        public readonly D ExactlyOneBindingTargetNotFound = new(Error, 205,
            "Exactly one binding target not found",
            "{0} binding targets were found for {1}, but exactly one is required");
        public readonly D DictionaryBindingMustHaveKey = new(Error, 206,
            "Dictionary binding must have a key attribute",
            "Dictionary binding on {0} must have the key specified on the attribute");
        public readonly D UnsupportedKeyType = new(Error, 207,
            "Dictionary binding has unsupported key type",
            "Dictionary binding {0} has unsupported key type {1}");
        public readonly D BindingMissingKey = new(Error, 208,
            "Dictionary binding target missing key",
            "Dictionary binding target {0} is missing a key for source {1}");
        public readonly D BindingNotValidInContext = new(Error, 209,
            "Binding not valid in context",
            "Binding {0} is not valid in one or more contexts");
        public readonly D UnknownBindingFactory = new(Error, 210,
            "Unknown binding factory",
            "Unknown binding factory {0}");

        ////////////////////////////////////////////////////////////////////////
        // Validator Diagnostics
        public readonly D OneRequired = new(Error, 300,
            "Exactly one required",
            "Exactly one {0} required in {1}");
        public readonly D IsPublic = new(Error, 301,
            "Must be public",
            "{0} must be public");
        public readonly D IsClass = new(Error, 302,
            "Must be a class",
            "{0} must be a class");
        public readonly D IsNotGeneric = new(Error, 303,
            "Must not be generic",
            "{0} must not be generic");
        public readonly D IsNotAbstract = new(Error, 304,
            "Must not be abstract",
            "{0} must not be abstract");
        public readonly D Extends = new(Error, 305,
            "Must extend",
            "{0} must extend {1}");
        public readonly D ContextTrue = new(Error, 306,
            "Context must be true",
            "{0}");
        public readonly D IsInstance = new(Error, 307,
            "Must not be static",
            "{0} must not be static");
        public readonly D IsOrdinaryMethod = new(Error, 308,
            "Must be ordinary method",
            "{0} must be an ordinary method");
        public readonly D IsParameterless = new(Error, 309,
            "Must be parameterless",
            "{0} must be a parameterless method");
        public readonly D IsDefaultConstructible = new(Error, 310,
            "Must be default-constructible",
            "{0} must have a public, parameterless constructor");
        public readonly D HasPublicGetterSetter = new(Error, 311,
            "Must have a public getter/setter",
            "{0} must have a public {1}");
        public readonly D Implements = new(Error, 312,
            "Must implement",
            "{0} must implement {1}");
        public readonly D IsArray = new(Error, 313,
            "Must be array",
            "{0} must be an array type");
    }
}
