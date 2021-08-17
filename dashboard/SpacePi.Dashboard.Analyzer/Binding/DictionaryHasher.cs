using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// A helper to generate hashes for a static dictionary
    /// </summary>
    /// <typeparam name="TKey">The type of key</typeparam>
    /// <typeparam name="TValue">The type of value</typeparam>
    public class DictionaryHasher<TKey, TValue> : Dictionary<TypedConstant, TValue>, IDictionaryHasher<TValue> {
        /// <summary>
        /// A helper to compare the equality of two dictionary keys in a non-generic way
        /// </summary>
        private class EqualityComparer : IEqualityComparer<TypedConstant> {
            /// <summary>
            /// The function to check for equality
            /// </summary>
            private readonly Func<TypedConstant, TypedConstant, bool> EqualsFunc;
            /// <summary>
            /// The function to compute a hash code
            /// </summary>
            private readonly Func<TypedConstant, int> GetHashCodeFunc;

            /// <summary>
            /// Checks for equality
            /// </summary>
            /// <param name="x">The first object</param>
            /// <param name="y">The second object</param>
            /// <returns>If they are equal</returns>
            public bool Equals(TypedConstant x, TypedConstant y) => EqualsFunc(x, y);

            /// <summary>
            /// Computes a hash code
            /// </summary>
            /// <param name="obj">The object to hash</param>
            /// <returns>The hash code</returns>
            public int GetHashCode(TypedConstant obj) => GetHashCodeFunc(obj);

            /// <summary>
            /// Creates a new EqualityComparer
            /// </summary>
            /// <typeparam name="Type">The key type to create</typeparam>
            /// <param name="comparer">The source comparer</param>
            /// <returns>The EqualityComparer</returns>
            public static EqualityComparer Create<Type>(IEqualityComparer<Type> comparer) => new((x, y) => comparer.Equals((Type) x.Value, (Type) y.Value), obj => comparer.GetHashCode((Type) obj.Value));

            /// <summary>
            /// Creates a new EqualityComparer
            /// </summary>
            /// <param name="equals">The function to check for equality</param>
            /// <param name="getHashCode">The function to compute a hash code</param>
            private EqualityComparer(Func<TypedConstant, TypedConstant, bool> equals, Func<TypedConstant, int> getHashCode) {
                EqualsFunc = equals;
                GetHashCodeFunc = getHashCode;
            }
        }

        /// <summary>
        /// The equality comparer
        /// </summary>
        private readonly IEqualityComparer<TKey> EqComparer;

        /// <summary>
        /// Hashes the data and computes the table to pass into a static dictionary constructor
        /// </summary>
        /// <returns>The table</returns>
        public (int, TypedConstant, TValue)[] GenerateTable() => new StaticDictionary<TypedConstant, TValue>(EqualityComparer.Create(EqComparer), this).Table;

        /// <summary>
        /// Rehashes the a new dictionary with a different value type
        /// </summary>
        /// <typeparam name="TOtherValue">The other value type</typeparam>
        /// <param name="dict">The new dictionary to rehash</param>
        /// <returns>The rehashed dictionary</returns>
        public IDictionaryHasher<TOtherValue> Rehash<TOtherValue>(IDictionary<TypedConstant, TOtherValue> dict) => new DictionaryHasher<TKey, TOtherValue>(dict, EqComparer);

        /// <summary>
        /// Creates a new DictionaryHasher
        /// </summary>
        /// <param name="dict">The dictionary to copy from</param>
        /// <param name="comparer">The equality comparer</param>
        public DictionaryHasher(IDictionary<TypedConstant, TValue> dict, IEqualityComparer<TKey> comparer) : base(dict) => EqComparer = comparer;

        /// <summary>
        /// Creates a new DictionaryHasher
        /// </summary>
        /// <param name="comparer">The equality comparer</param>
        public DictionaryHasher(IEqualityComparer<TKey> comparer) => EqComparer = comparer;
    }
}
