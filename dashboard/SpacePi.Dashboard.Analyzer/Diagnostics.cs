using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Text;
using SpacePi.Dashboard.API;

namespace SpacePi.Dashboard.Analyzer {
    static class Diagnostics {
        public static DiagnosticDescriptor ProtocBuildFailed = new(
            "SPI0001",
            "Protoc plugin build failed",
            $"Failed to build {BuildConfig.protoc_gen_spacepi_csharp.TARGET_NAME_IF_EXISTS} target using CMake",
            "SpacePi",
            DiagnosticSeverity.Error,
            true);
        public static DiagnosticDescriptor ProtocFailure = new(
            "SPI0002",
            "Protoc failure",
            "Protoc reported a build failure",
            "SpacePi",
            DiagnosticSeverity.Error,
            true);
        public static DiagnosticDescriptor UnknownBindingClass = new(
            "SPI0003",
            "Unknown binding class",
            "The binding target class {0} for field {1}.{2} was not found",
            "SpacePi",
            DiagnosticSeverity.Error,
            true);
        public static DiagnosticDescriptor AnalyzerException = new(
            "SPI0004",
            "Analyzer encountered an exception",
            "Analyzer encountered an exception of type '{0}' with message '{1}'",
            "SpacePi",
            DiagnosticSeverity.Error,
            true);
    }
}
