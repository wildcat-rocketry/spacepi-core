using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public class Diag {
        private static int LastID = 0;
        public static readonly List<DiagnosticDescriptor> All = new();
        public readonly DiagnosticDescriptor Descriptor;

        public Diagnostic Create(Location loc, params object[] args) => Diagnostic.Create(Descriptor, loc, args);

        public Diagnostic Create(ISymbol sym, params object[] args) => Create(sym.Locations.FirstOrDefault(), args);

        public Diagnostic Create(params object[] args) => Create((Location) null, args);

        public Diag(string @short, string format, DiagnosticSeverity sev, int skip = 0) {
            LastID += skip + 1;
            Descriptor = new($"SPI{LastID:D4}", @short, format, "SpacePi", sev, true);
            All.Add(Descriptor);
        }
    }
}
