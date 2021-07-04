using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;
using Microsoft.CodeAnalysis;

[assembly: SuppressMessage("MicrosoftCodeAnalysisCorrectness",
    "RS2002:Do not add removed analyzer diagnostic IDs to unshipped analyzer release",
    Justification = "Analyzer doesn't support helper functions for Diag constructor")]

namespace SpacePi.Dashboard.Analyzer {
    public static class Diagnostics {
        public static readonly Diag AnalyzerException = new(
            "Analyzer encountered an exception",
            "Analyzer encountered an exception of type '{0}' with message '{1}'",
            DiagnosticSeverity.Error);

        public static readonly Diag CMakeBuildStatus = new(
            "CMake build message",
            "{0}",
            DiagnosticSeverity.Info);

        public static readonly Diag CMakeBuildError = new(
            "CMake build error",
            "{0}",
            DiagnosticSeverity.Error);

        public static readonly Diag MissingAPIType = new(
            "Missing API type",
            "Required API type {0} not found.",
            DiagnosticSeverity.Error);

        public static readonly Diag ProtocBuildStatus = new(
            "Protobuf build message",
            "{0}",
            DiagnosticSeverity.Info);

        public static readonly Diag ProtocBuildError = new(
            "Protobuf build error",
            "{0}",
            DiagnosticSeverity.Error);

        public static readonly Diag PluginNotAttributed = new(
            "Plugin does not have PluginAttribute",
            "Plugin '{0}' does not have a PluginAttribute",
            DiagnosticSeverity.Warning);

        public static readonly Diag PluginMissingInterface = new(
            "Plugin does not implement IPlugin",
            "Plugin '{0}' does not implement IPlugin",
            DiagnosticSeverity.Error);

        public static readonly Diag PluginNotClass = new(
            "Plugin is not a class",
            "Plugin '{0}' is not a class type",
            DiagnosticSeverity.Error);

        public static readonly Diag PluginIsAbstract = new(
            "Plugin is abstract",
            "Plugin '{0}' is an abstract class",
            DiagnosticSeverity.Error);

        public static readonly Diag PluginIsGeneric = new(
            "Plugin is generic",
            "Plugin '{0}' is a generic type",
            DiagnosticSeverity.Error);

        public static readonly Diag PluginHasBadConstructor = new(
            "Plugin missing constructor",
            "Plugin '{0}' does not have a public, parameterless constructor",
            DiagnosticSeverity.Error);

        public static readonly Diag PluginHasDuplicatePriority = new(
            "Two plugins have the same priority",
            "Plugin '{0}' and '{1}' both have the same priority value {2}",
            DiagnosticSeverity.Warning);

        public static readonly Diag ClassBindingNotFound = new(
            "Binding has no target",
            "Binding '{0}.{1}' is invalid because '{2}' is not a valid target",
            DiagnosticSeverity.Error);

        public static readonly Diag InterfaceBindingNotFound = new(
            "Binding has no target",
            "Binding '{0}.{1}' is invalid because interface '{2}' has no valid implementations",
            DiagnosticSeverity.Error);

        public static readonly Diag InterfaceBindingFound = new(
            "Binding has no target",
            "Binding '{0}.{1}' is invalid because interface '{2}' has multiple implementations",
            DiagnosticSeverity.Error);

        public static readonly Diag ArrayBindingMustBeInterface = new(
            "Binding has no target",
            "Binding '{0}.{1}' is invalid because IEnumerable<> bindings must be of an interface type, but '{2}' is not",
            DiagnosticSeverity.Error);

        public static readonly Diag BindingMustBeProperty = new(
            "Binding must be a property",
            "Binding '{0}.{1}' is invalid because all bindings must be on a property",
            DiagnosticSeverity.Warning);

        public static readonly Diag BoundPropertyNeedsPublicSet = new(
            "Bound property must have public setter",
            "Binding '{0}.{1}' is invalid because it does not have a public setter",
            DiagnosticSeverity.Error);

        public static readonly Diag ContextNotClass = new(
            "Context is not a class",
            "Context '{0}' is not a class type",
            DiagnosticSeverity.Error);

        public static readonly Diag ContextMissingInterface = new(
            "Context does not implement IContext",
            "Context '{0}' does not implement IContext",
            DiagnosticSeverity.Error);

        public static readonly Diag ContextDuplicated = new(
            "Context duplicated between plugins",
            "Context '{0}' is duplicated between plugins '{1}' and '{2}'",
            DiagnosticSeverity.Error);

        public static readonly Diag ContextNotRegistered = new(
            "Context is not registered to any plugin",
            "Context '{0}' is not registered to any plugin",
            DiagnosticSeverity.Warning);
    }
}
