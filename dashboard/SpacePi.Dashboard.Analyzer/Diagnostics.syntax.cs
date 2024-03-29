﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// The diagnostics which can be reported by the analyzer
    /// </summary>
    public partial class Diagnostics : IEnumerable<Diagnostic> {
        /// <summary>
        /// Base class for a type of diagnostic
        /// </summary>
        public abstract class Type {
            /// <summary>
            /// Reports the diagnostic
            /// </summary>
            /// <param name="loc">The location in source at which to report it</param>
            /// <param name="args">The formatting arguments to the diagnostic message</param>
            public abstract void Report(Location loc, params object[] args);

            /// <summary>
            /// Reports the diagnostic
            /// </summary>
            /// <param name="sym">The symbol at whihc to report it</param>
            /// <param name="args">The formatting arguments to the diagnostic message</param>
            public void Report(ISymbol sym, params object[] args) => Report(sym.Locations.FirstOrDefault(), args);

            /// <summary>
            /// Reports the diagnostic
            /// </summary>
            /// <param name="args">The formatting arguments to the diagnostic message</param>
            public void Report(params object[] args) => Report((Location) null, args);
        };

        /// <summary>
        /// Implementation for a type of diagnostic
        /// </summary>
        public class D : Type {
            /// <summary>
            /// The already created diagnostic objects
            /// </summary>
            private static readonly Dictionary<int, DiagnosticDescriptor> Descriptors = new();
            /// <summary>
            /// The roslyn diagnostic instance
            /// </summary>
            private readonly DiagnosticDescriptor Descriptor;
            /// <summary>
            /// A list of reported instances
            /// </summary>
            private readonly List<Diagnostic> Instances;

            /// <summary>
            /// Reports the diagnostic
            /// </summary>
            /// <param name="loc">The location in source at which to report it</param>
            /// <param name="args">The formatting arguments to the diagnostic message</param>
            public override void Report(Location loc, params object[] args) => Instances.Add(Diagnostic.Create(Descriptor, loc, args));

            /// <summary>
            /// Creates a new type of diagnostic
            /// </summary>
            /// <param name="defaultSeverity">The default severity of the diagnostic</param>
            /// <param name="id">The ID number of the diagnostic</param>
            /// <param name="title">The title of the diagnostic</param>
            /// <param name="messageFormat">The format string used to format the diagnostic</param>
            public D(DiagnosticSeverity defaultSeverity, int id, string title, string messageFormat) {
                if (!Descriptors.TryGetValue(id, out Descriptor)) {
                    Descriptor = new($"SPI{id:D4}", title, messageFormat, "SpacePi", defaultSeverity, true);
                    Descriptors[id] = Descriptor;
                }
                Instances = NewInstances;
            }
        }

        [SuppressMessage("CodeQuality", "IDE0051:Remove unused private members", Justification = "Kept for future use")]
        private const DiagnosticSeverity Hidden = DiagnosticSeverity.Hidden;
        private const DiagnosticSeverity Info = DiagnosticSeverity.Info;
        [SuppressMessage("CodeQuality", "IDE0051:Remove unused private members", Justification = "Kept for future use")]
        private const DiagnosticSeverity Warning = DiagnosticSeverity.Warning;
        private const DiagnosticSeverity Error = DiagnosticSeverity.Error;

        /// <summary>
        /// New instances which will be generated by the next Diagnostics object constructed
        /// </summary>
        private static List<Diagnostic> NewInstances = new();
        /// <summary>
        /// The instances generated by this Diagnostics
        /// </summary>
        private readonly List<Diagnostic> Instances;

        /// <summary>
        /// Gets an enumerator of reported diagnostics
        /// </summary>
        /// <returns>The enumerator</returns>
        public IEnumerator<Diagnostic> GetEnumerator() => Instances.GetEnumerator();

        /// <summary>
        /// Gets an enumerator of reported diagnostics
        /// </summary>
        /// <returns>The enumerator</returns>
        IEnumerator IEnumerable.GetEnumerator() => Instances.GetEnumerator();

        /// <summary>
        /// Creates a new Diagnostics
        /// </summary>
        public Diagnostics() {
            Instances = NewInstances;
            NewInstances = new();
        }
    }
}
