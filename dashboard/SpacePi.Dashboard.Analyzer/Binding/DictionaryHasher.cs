using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public class DictionaryHasher<Type> : Dictionary<TypedConstant, FactoryObject>, IDictionaryHasher {
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

        private readonly IEqualityComparer<Type> EqComparer;

        public (int, TypedConstant, FactoryObject)[] GenerateTable() => new StaticDictionary<TypedConstant, FactoryObject>(EqualityComparer.Create(EqComparer), this).Table;

        public DictionaryHasher(IEqualityComparer<Type> comparer) => EqComparer = comparer;
    }
}
