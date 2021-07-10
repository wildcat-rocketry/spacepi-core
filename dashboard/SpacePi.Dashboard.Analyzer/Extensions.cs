using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public static class Extensions {
        public static INamedTypeSymbol InCompilation(this Type type, Compilation comp, Diagnostics diags) {
            INamedTypeSymbol sym = comp.GetTypeByMetadataName(type.FullName);
            if (sym == null) {
                diags.APITypeMissing.Report(type.FullName);
            }
            return sym;
        }
    }
}
