using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public interface IDictionaryHasher : IDictionary<TypedConstant, FactoryObject> {
        (int, TypedConstant, FactoryObject)[] GenerateTable();
    }
}
