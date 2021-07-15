using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record GeneratedClass {
        private static int Nonce = 0;

        public static readonly string Namespace = $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Analyzer)}.{nameof(Binding)}.Generated";
        public readonly string ClassName = $"G{Nonce++:D7}";
    }
}
