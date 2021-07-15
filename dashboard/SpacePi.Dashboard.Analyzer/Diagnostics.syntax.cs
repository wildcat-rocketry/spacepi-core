using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public partial class Diagnostics : IEnumerable<Diagnostic> {
        public abstract class Type {
            public abstract void Report(Location loc, params object[] args);

            public void Report(ISymbol sym, params object[] args) => Report(sym.Locations.FirstOrDefault(), args);

            public void Report(params object[] args) => Report((Location) null, args);
        };

        public class D : Type {
            private static readonly Dictionary<int, DiagnosticDescriptor> Descriptors = new();
            private readonly DiagnosticDescriptor Descriptor;
            private readonly List<Diagnostic> Instances;

            public override void Report(Location loc, params object[] args) => Instances.Add(Diagnostic.Create(Descriptor, loc, args));

            public D(DiagnosticSeverity defaultSeverity, int id, string title, string messageFormat) {
                if (!Descriptors.TryGetValue(id, out Descriptor)) {
                    Descriptor = new($"SPI{id:D4}", title, messageFormat, "SpacePi", defaultSeverity, true);
                    Descriptors[id] = Descriptor;
                }
                Instances = NewInstances;
            }
        }

        private static readonly DiagnosticSeverity Hidden = DiagnosticSeverity.Hidden;
        private static readonly DiagnosticSeverity Info = DiagnosticSeverity.Info;
        private static readonly DiagnosticSeverity Warning = DiagnosticSeverity.Warning;
        private static readonly DiagnosticSeverity Error = DiagnosticSeverity.Error;

        private static List<Diagnostic> NewInstances = new();
        private readonly List<Diagnostic> Instances;

        public IEnumerator<Diagnostic> GetEnumerator() => Instances.GetEnumerator();

        IEnumerator IEnumerable.GetEnumerator() => Instances.GetEnumerator();

        public Diagnostics() {
            Instances = NewInstances;
            NewInstances = new();
        }
    }
}
