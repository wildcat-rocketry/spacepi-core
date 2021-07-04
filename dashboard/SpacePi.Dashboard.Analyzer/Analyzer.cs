using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public abstract class Analyzer {
        private readonly ThreadLocal<Stack<List<Diagnostic>>> Diagnostics = new();
        protected readonly Compilation Compilation;

        protected void ReportDiagnostic(Diagnostic diag) {
            List<Diagnostic> diags = null;
            if (Diagnostics.IsValueCreated) {
                diags = Diagnostics.Value.FirstOrDefault();
            }
            diags?.Add(diag);
        }

        protected IEnumerable<Diagnostic> RunWithDiagnostics(Action action) {
            if (!Diagnostics.IsValueCreated) {
                Diagnostics.Value = new();
            }
            Diagnostics.Value.Push(new());
            action();
            return Diagnostics.Value.Pop();
        }

        public INamedTypeSymbol ResolveType(string fqcn) {
            INamedTypeSymbol sym = Compilation.GetTypeByMetadataName(fqcn);
            if (sym == null) {
                ReportDiagnostic(Dashboard.Analyzer.Diagnostics.MissingAPIType.Create(fqcn));
            }
            return sym;
        }

        public Analyzer(Compilation compilation) => Compilation = compilation;
    }
}
