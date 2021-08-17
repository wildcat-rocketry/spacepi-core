using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// General extension methods used by the analyzer
    /// </summary>
    public static class Extensions {
        /// <summary>
        /// Checks if two symbols are equal based on the default comparison strategy
        /// </summary>
        /// <param name="lhs">The first symbol</param>
        /// <param name="rhs">The second symbol</param>
        /// <returns>If they are equal</returns>
        public static bool DEquals(this ISymbol lhs, ISymbol rhs) => SymbolEqualityComparer.Default.Equals(lhs, rhs);

        /// <summary>
        /// Gets all members defined in type and its descendant types
        /// </summary>
        /// <param name="symbol">The symbol</param>
        /// <returns>All members contained within the symbol or its descendants</returns>
        public static IEnumerable<ISymbol> GetAllMembers(this ITypeSymbol symbol) => symbol.GetMembers().Concat(symbol.BaseType?.GetAllMembers() ?? Enumerable.Empty<ISymbol>());
    }
}
