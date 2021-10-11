using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// Equality comparers for primitive types
    /// </summary>
    public static class PrimitiveEqualityComparers {
        public class String : IEqualityComparer<string> {
            public bool Equals(string x, string y) => x.Equals(y);

            public int GetHashCode(string obj) => obj.Select(c => (int) c).Aggregate(7, (a, b) => 31 * a + b);
        }

        public class Int : IEqualityComparer<int> {
            public bool Equals(int x, int y) => x == y;

            public int GetHashCode(int obj) => obj;
        }
    }
}
