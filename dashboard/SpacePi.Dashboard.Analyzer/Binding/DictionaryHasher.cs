using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public class DictionaryHasher<TKey, TValue> : Dictionary<TypedConstant, TValue>, IDictionaryHasher<TValue> {
        private class EqualityComparer : IEqualityComparer<TypedConstant> {
            private readonly Func<TypedConstant, TypedConstant, bool> EqualsFunc;
            private readonly Func<TypedConstant, int> GetHashCodeFunc;

            public bool Equals(TypedConstant x, TypedConstant y) => EqualsFunc(x, y);

            public int GetHashCode(TypedConstant obj) => GetHashCodeFunc(obj);

            public static EqualityComparer Create<Type>(IEqualityComparer<Type> comparer) => new((x, y) => comparer.Equals((Type) x.Value, (Type) y.Value), obj => comparer.GetHashCode((Type) obj.Value));

            private EqualityComparer(Func<TypedConstant, TypedConstant, bool> equals, Func<TypedConstant, int> getHashCode) {
                EqualsFunc = equals;
                GetHashCodeFunc = getHashCode;
            }
        }

        private readonly IEqualityComparer<TKey> EqComparer;

        public (int, TypedConstant, TValue)[] GenerateTable() => new StaticDictionary<TypedConstant, TValue>(EqualityComparer.Create(EqComparer), this).Table;

        public IDictionaryHasher<TOtherValue> Rehash<TOtherValue>(IDictionary<TypedConstant, TOtherValue> dict) => new DictionaryHasher<TKey, TOtherValue>(dict, EqComparer);

        public DictionaryHasher(IDictionary<TypedConstant, TValue> dict, IEqualityComparer<TKey> comparer) : base(dict) => EqComparer = comparer;

        public DictionaryHasher(IEqualityComparer<TKey> comparer) => EqComparer = comparer;
    }
}
