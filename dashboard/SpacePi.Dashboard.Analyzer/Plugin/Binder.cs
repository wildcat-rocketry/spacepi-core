using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public class Binder<TTarget> where TTarget : IEquatable<TTarget> {
        private readonly Dictionary<TTarget, TTarget> Targets = new();

        public void AddTarget(TTarget target) => Targets.Add(target, target);

        public IEnumerable<Diagnostic> Bind(IEnumerable<Binding<TTarget>> bindings) {
            foreach (Binding<TTarget> binding in bindings) {
                foreach (Diagnostic diag in binding.TryBind(Targets)) {
                    yield return diag;
                }
            }
        }
    }
}
