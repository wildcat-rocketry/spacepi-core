using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public interface IDictionaryHasher<TValue> : IDictionary<TypedConstant, TValue> {
        (int, TypedConstant, TValue)[] GenerateTable();

        IDictionaryHasher<TOtherValue> Rehash<TOtherValue>(IDictionary<TypedConstant, TOtherValue> dict);
    }
}
