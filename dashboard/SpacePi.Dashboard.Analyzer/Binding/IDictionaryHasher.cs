using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    /// <summary>
    /// Interface to generate hashes for a static dictionary
    /// </summary>
    /// <typeparam name="TValue">The type of the value</typeparam>
    public interface IDictionaryHasher<TValue> : IDictionary<TypedConstant, TValue> {
        /// <summary>
        /// Hashes the data and computes the table to pass into a static dictionary constructor
        /// </summary>
        /// <returns>The table</returns>
        (int, TypedConstant, TValue)[] GenerateTable();

        /// <summary>
        /// Rehashes the a new dictionary with a different value type
        /// </summary>
        /// <typeparam name="TOtherValue">The other value type</typeparam>
        /// <param name="dict">The new dictionary to rehash</param>
        /// <returns>The rehashed dictionary</returns>
        IDictionaryHasher<TOtherValue> Rehash<TOtherValue>(IDictionary<TypedConstant, TOtherValue> dict);
    }
}
