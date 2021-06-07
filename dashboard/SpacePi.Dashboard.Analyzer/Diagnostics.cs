using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
using System.Text;

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
    }
}
