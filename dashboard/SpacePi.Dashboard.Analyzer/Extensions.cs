using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public static class Extensions {
        public static bool DEquals(this ISymbol lhs, ISymbol rhs) => SymbolEqualityComparer.Default.Equals(lhs, rhs);

        public static IEnumerable<ISymbol> GetAllMembers(this ITypeSymbol symbol) => symbol.GetMembers().Concat(symbol.BaseType?.GetAllMembers() ?? Enumerable.Empty<ISymbol>());
    }
}
